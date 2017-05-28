#include "Text_Printer.h"

std::vector<std::array<SDL_Texture*, 127>> Text_Printer::lettersVec;
std::vector<NodeQueue> Text_Printer::queue;
Uint32 Text_Printer::timerA;
Uint32 Text_Printer::timerB;
SDL_Rect Text_Printer::defaultRect;
SDL_Rect Text_Printer::defaultContainer;
int Text_Printer::flagOverflow;
SDL_Texture* Text_Printer::dialogBox = NULL;
SDL_Rect Text_Printer::dialogRect;
bool Text_Printer::busy = 0;

void Text_Printer::printText(NodeQueue& node) {
	SDL_Rect blitRect = node.container;
	blitRect.w = node.rect.w; blitRect.h = node.rect.h;
	for (auto i = 0; i <= node.iterator; i++) {
		//check if there is enough space for the word, if not we jump a line
		if (i == 0 || node.str[i] == ' ') {
			int y = i;
			while (node.str[y] == ' ') y++;
			while (node.str[y] != ' ' && node.str[y] != '\0')
				y++;
			//if out of limit
			if (blitRect.x + ((y - i)*blitRect.w) > node.container.w + node.container.x) {
				if (blitRect.y + 2 * blitRect.h > node.container.h + node.container.y) {
					node.lock = 1;
					return;
				}
				//jump line
				else {
					blitRect.y += blitRect.h;
					blitRect.x = node.container.x;
					while (node.str[i] == ' ') {
						i++;
						if (i > node.iterator)
							break;
					}
				}
			}
		}
		SDL_RenderCopy(Renderer::g_Renderer, lettersVec[node.policeID][node.str[i]],
			NULL, &blitRect);
		blitRect.x += blitRect.w;
	}
	if (node.str[node.iterator] == '\0') node.lock = 1;
}

void Text_Printer::addToQueue(std::string str,
	SDL_Rect* container, int policeID, SDL_Rect* rect)
{
	if (!str.size()) return;
	if (queue.size() > 10000) {
		if (!flagOverflow) {
			flagOverflow = 1;
			std::cout << "Beaucoup trop de texte en queue : " << queue.size() << std::endl;
		}
		return;
	}
	else flagOverflow = 0;
	if (rect == NULL) rect = &defaultRect;
	if (container == NULL) container = &defaultContainer;
	if (policeID > lettersVec.size() - 1) {
		std::cout << "Invalid index for Police ! index : " << policeID << std::endl;
		return;
	}
	NodeQueue tempNode{ str, *rect, policeID, 0 , *container, 0 };
	queue.push_back(tempNode);
}

void Text_Printer::handleRoutine(SDL_Event e)
{
	timerB = SDL_GetTicks();
	if (queue.size() > 0) {
		busy = 1;
		SDL_RenderCopy(Renderer::g_Renderer, dialogBox,
			NULL, &dialogRect);
		printText(queue[0]);
		if (queue[0].lock) {
			//print a ->
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_j) {
					while (queue[0].str[queue[0].iterator] == ' ')
						queue[0].iterator++;
					queue[0].str.erase(0, queue[0].iterator);
					queue[0].lock = 0;
					queue[0].iterator = 0;
					if (queue[0].iterator == queue[0].str.size())
						queue.erase(queue.begin(), queue.begin() + 1);
				}
			}
			return;
		}
		if (timerB - timerA > PRINT_SPEED && queue[0].iterator != queue[0].str.size()) {
			queue[0].iterator++;
			timerA = timerB = SDL_GetTicks();
			return;
		}
	}
	else busy = 0;
}

void Text_Printer::Init() {
	std::ifstream tempStream = loadFile(Paths::asciiPath);
	if (!tempStream) {
		printf("ERROR : policeList not loaded\n");
		return;
	}

	std::string catcher;
	std::vector<TTF_Font*> tempVec;
	tempStream.clear();
	tempStream.seekg(0);
	while (!tempStream.eof()) {
		getline(tempStream, catcher);
		catcher.insert(0, "./Polices/");
		std::cout << catcher << std::endl;
		tempVec.push_back(TTF_OpenFont(catcher.c_str(), 200));
	}

	char tempStr[2] = "A";
	tempStr[1] = '\0';
	SDL_Color textColor = { 255, 255, 255 };
	SDL_Surface* textSurface = NULL;
	std::array<SDL_Texture*, 127> tempLetters = { NULL };

	for (auto i = tempVec.begin(); i != tempVec.end(); ++i) {
		for (int y = 0; y < 127; y++) {
			tempStr[0] = y;
			textSurface = TTF_RenderText_Solid(*i, tempStr, textColor);
			tempLetters[y] = SDL_CreateTextureFromSurface(Renderer::g_Renderer, textSurface);
		}
		lettersVec.push_back(tempLetters);
	}

	timerA = timerB = SDL_GetTicks();
	defaultRect.h = 8; defaultRect.x = 30;
	defaultRect.w = 8; defaultRect.y = 30;
	defaultContainer.h = 28; defaultContainer.x = 79;
	defaultContainer.w = 220; defaultContainer.y = 208;
	flagOverflow = 0;
	dialogBox = Textures_Manager::findTexture("dialog_box_clean.png");
	dialogRect.x = 0; dialogRect.y = 200;
	dialogRect.h = 40; dialogRect.w = 320;
}