#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <string>
#include <cstdio>
#include <iostream>
#include "kreator.h"

using namespace sf;
using namespace std;

//wybor serwer czy klient
bool serwer;
//czyja tura
bool sturn=true;
//rozmiar okna programu
int WIDTH = 600;
int HEIGHT = 315;
//Port i IP do zmiany, obecnie localhost
const unsigned short PORT = 5000;
std::string IPADDRESS("127.0.0.1");

//zmienne chatu
std::string msgSend;
TcpSocket socket;
Mutex globalMutex;
bool quit = false;

void DoStuff(void)
{
	static std::string oldMsg;
	while (!quit)
	{
		sf::Packet packetSend;
		globalMutex.lock();
		packetSend << msgSend;
		globalMutex.unlock();

		socket.send(packetSend);

		std::string msg;
		sf::Packet packetReceive;

		socket.receive(packetReceive);
		if (packetReceive >> msg)
		{
			if (oldMsg != msg)
				if (!msg.empty())
				{
					std::cout << msg << std::endl;
					oldMsg = msg;
				}
		}
	}
}
void Server(void)
{
	sf::TcpListener listener;
	listener.listen(PORT);
	listener.accept(socket);
	std::cout << "Do chatu dolaczyla nowa osoba: " << socket.getRemoteAddress() << std::endl;
}
bool Client(void)
{
	if (socket.connect(IPADDRESS, PORT) == sf::Socket::Done)
	{
		std::cout << "Polaczono\n";
		return true;
	}
	return false;
}
void GetInput(void)
{
	std::string s;
	std::cin >> s;
	if (s == "exit")
		quit = true;
	globalMutex.lock();
	msgSend = s;
	globalMutex.unlock();
}
void mainChat(void){
	sf::Thread* thread = 0;
	if (serwer== true)
		Server();
	else
		Client();

	thread = new sf::Thread(&DoStuff);
	thread->launch();
	while (!quit)
	{
		GetInput();
	}
	if (thread)
	{
		thread->wait();
		delete thread;
	}
}

int main()
{
	/////////////
	//Kreator Postaci
	/////////////
	cout<<"Przed rozpoczeciem rozgrywki stworz swoja postac: ";
	cout<<"\nChcesz stworzyc nowa czy wczytac stara? n/s: ";
	char des;
	cin>>des;
	if (des=='n'){
		kreator hero;
		hero.zapiszDoPliku();
	}
	else
	{
		kreator hero("abc", 10,10,10);
		hero.wczytajZPliku();
	}

	//Ustawienia okna programu
	//Antyaliasing =8
	ContextSettings settings;
	settings.antialiasingLevel=8;
	
	//Okno programu
	//okno pytajace czy jestesmy klientem czy serwerem
	RenderWindow window(VideoMode(WIDTH,HEIGHT),"Kamil Luc - TUROWA GRA RPG", Style::Close, settings);

	//Napis z pytaniem o k/s
	Font font;
	font.loadFromFile("../Release/Images/robotastic.ttf");
	Text napis("Serwer [S]/Klient [K]",font,20);
	napis.setPosition(WIDTH/2-napis.getGlobalBounds().width/2,(HEIGHT/2-napis.getGlobalBounds().height/2));

	/////////////////////////
	// Petla glowna 1'go okna
	/////////////////////////
	while(true)
	{
		Event event;
		while(window.pollEvent(event));
		{
			if(event.type == Event::Closed)
				return 0;
			if(event.type == Event::KeyReleased && event.key.code == Keyboard::S)
			{
				serwer = true;
				break;
			}
			if(event.type == Event::KeyReleased && event.key.code == Keyboard::K)
			{
				serwer = false;
				break;
			}
		}
		window.clear();
		window.draw(napis);
		window.display();
	}


	//zmiana napisu na pytanie o ip
	napis.setString("Podaj ip klienta/serwera");
	napis.setPosition(WIDTH/2-napis.getGlobalBounds().width/2,(HEIGHT/2-napis.getGlobalBounds().height/2)-30);
	Text text_ip("127.0.0.1",font,15);
	std::string str_ip("127.0.0.1");
	IPADDRESS = str_ip;
	bool b = false;

	/////////////////////////
	// Petla glowna 2'go okna
	/////////////////////////
		while(!b)
		{
			Event event;
			while(window.pollEvent(event))
			{
				if(event.type == Event::Closed)
					return 0;
				if(event.type == Event::KeyReleased && event.key.code == Keyboard::Return)
				{
					IPADDRESS = str_ip;
					b = true;
				}
				
				else if(event.type == Event::TextEntered)
				{
					if (event.text.unicode == 8) //backspace
					{
						if(str_ip.size()>0)
							str_ip.erase(str_ip.size() - 1, 1);
					}
					else if (event.text.unicode < 128 && event.text.unicode != 13)
						str_ip+= (char)event.text.unicode;
					
					text_ip.setString(str_ip);
					text_ip.setPosition(WIDTH/2-text_ip.getGlobalBounds().width/2,HEIGHT/2-text_ip.getGlobalBounds().height/2);
				}
			}	
			text_ip.setPosition(WIDTH/2-text_ip.getGlobalBounds().width/2,HEIGHT/2-text_ip.getGlobalBounds().height/2);
			window.clear();
			window.draw(napis);
			window.draw(text_ip);
			window.display();
		}
		//////////////



	//Zmiana rozmiaru okna dla gry
	HEIGHT=576;
	WIDTH=1024;
	Vector2u nowyrozmiar(WIDTH, HEIGHT);
	window.setSize(nowyrozmiar);
	window.setView(sf::View(sf::FloatRect(0, 0, (float)WIDTH, (float)HEIGHT)));

	//Obiekty na scenie
    CircleShape shape(100.f);
	CircleShape shape2(40.f);
    shape.setFillColor(sf::Color::Green);
	shape2.setFillColor(Color::Blue);
	
	//Kolor maski do tekstur
	Color color1(255,48,153);

	//image
	Image image1;
	image1.loadFromFile("../Release/Images/001.png");
	image1.createMaskFromColor(color1, 0);

	Image image2;
	image2.loadFromFile("../Release/Images/002.png");
	image2.createMaskFromColor(color1, 0);

	Image image3;
	image3.loadFromFile("../Release/Images/003.png");
	image3.createMaskFromColor(color1, 0);
	
	Image image4;
	image4.loadFromFile("../Release/Images/004.png");
	image4.createMaskFromColor(color1, 0);

	Image image5;
	image5.loadFromFile("../Release/Images/005.png");
	image5.createMaskFromColor(color1, 0);

	Image image6;
	image6.loadFromFile("../Release/Images/006.png");
	image6.createMaskFromColor(color1, 0);
	
	//textura
	Texture texture1;
	texture1.loadFromImage(image1);
	texture1.setSmooth(true);

	Texture texture2;
	texture2.loadFromImage(image2);
	texture2.setSmooth(true);

	Texture texture3;
	texture3.loadFromImage(image3);
	texture3.setSmooth(true);
	texture3.setRepeated(true);

	Texture texture4;
	texture4.loadFromImage(image4);
	texture4.setSmooth(true);

	Texture texture5;
	texture5.loadFromImage(image5);
	texture5.setSmooth(true);

	Texture texture6;
	texture6.loadFromImage(image6);
	texture6.setSmooth(true);

	Texture texture7;
	texture7.loadFromFile("../Release/Images/007.png");
	texture7.setSmooth(true);


	//sprite
	//hero1
	Sprite sprite1;
	sprite1.setTexture(texture1);
	//hero2
	Sprite sprite2;
	sprite2.setTexture(texture2);
	//trawa
	Sprite sprite3;
	sprite3.setTexture(texture3);
	sprite3.setTextureRect(IntRect(0,0,1024,576));
	//button1
	Sprite sprite4;
	sprite4.setTexture(texture4);
	sprite4.setPosition(Vector2f(462, 256+200));
	//button2
	Sprite sprite5;
	sprite5.setTexture(texture5);
	sprite5.setPosition(Vector2f(462, 292+200));
	//button3
	Sprite sprite6;
	sprite6.setTexture(texture6);
	sprite6.setPosition(Vector2f(462, 328+200));
	//hp1
	Sprite sprite7;
	sprite7.setTexture(texture7);
	sprite7.setPosition(Vector2f(0, 0));
	//hp2
	Sprite sprite8;
	sprite8.setTexture(texture7);
	sprite8.setPosition(Vector2f(1024-400, 0));



	
	/////////////////////
	///chat w innnym watku
	/////////////////////
	Thread thread0 = (&mainChat);
	thread0.launch();
	
	int hp1=100, hp2=100;
	/*
	Font font;
	font.loadFromFile("../Release/Images/robotastic.ttf");
	Text napis("Serwer [S]/Klient [K]",font,20);
	napis.setPosition(WIDTH/2-napis.getGlobalBounds().width/2,(HEIGHT/2-napis.getGlobalBounds().height/2));

	*/
	string tmp; // brzydkie rozwi¹zanie
	sprintf((char*)tmp.c_str(), "%d", hp1);
	string shp1 = tmp.c_str();

	sprintf((char*)tmp.c_str(), "%d", hp2);
	string shp2 = tmp.c_str();

	Font font2;
	font2.loadFromFile("../Release/Images/robotastic.ttf");

	Text text1;
	text1.setFont(font2);
	text1.setString(shp1);
	text1.setCharacterSize(24);
	text1.setColor(Color::Black);
	text1.setPosition((float)1, (float)1);
	Text text2;
	text2.setFont(font2);
	text2.setString(shp2);
	text2.setCharacterSize(24);
	text2.setColor(Color::Black);
	text2.setPosition((float)940, (float)1);

	if(serwer==false){
		
	text1.setPosition(940.0f, 1.0f);
	text2.setPosition(1.0f, 1.0f);
	}
	//stworzenie nowego socketu do gry
	TcpSocket gsck;
	if(serwer==true){
		TcpListener listener;
		listener.listen(2001);
		listener.accept(gsck);
	}
	else
		gsck.connect(IPADDRESS, 2001);

	gsck.setBlocking(false);

	TcpSocket gsck2;
	if(serwer==true){
		TcpListener listener;
		listener.listen(2002);
		listener.accept(gsck2);
	}
	else
		gsck2.connect(IPADDRESS, 2002);

	gsck2.setBlocking(false);

	//przechowuje pozycje gracza psp serwer pcp klient
	Vector2f psp, pcp;
	//info o ostatniej akcji(atak, leczenie) kolejno serwera i klienta
	sf::Uint16 psa=0, pca=0;
	//////////////////////////
	//Glowna petla dla serwera
	//////////////////////////
	if(serwer==true){
    while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
			//zczytuje pozycje serwera
			psp=sprite1.getPosition();
			
			//ruch postaci
			if(event.type==Event::KeyPressed)
			{
				if(event.key.code==Keyboard::W)
					sprite1.move(Vector2f(0,-35));
				else if(event.key.code==Keyboard::S)
					sprite1.move(Vector2f(0,35));
				else if(event.key.code==Keyboard::A)
					sprite1.move(Vector2f(-35,0));
				else if(event.key.code==Keyboard::D)
					sprite1.move(Vector2f(35,0));
			}

			//wyslanie i odbieranie pakietow
			Packet pck;
			if(psp!=sprite1.getPosition()){
				pck<<sprite1.getPosition().x<<sprite1.getPosition().y;
				gsck.send(pck);
			}

			gsck.receive(pck);
			if(pck>>pcp.x>>pcp.y){
				sprite2.setPosition(pcp);
			}

		
			psa=0;
			//klikanie w guziki odpowiadajace atakowi, obronie etc
			if(event.type == Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					Vector2i x=Mouse::getPosition(window);
					float xx=(float)x.x;
					float yy=(float)x.y;
					if(sprite4.getGlobalBounds().contains(xx,yy))
					{
						psa=1;
						sprite7.move(Vector2f(35,0));

						hp2-+5;
						tmp="";
						sprintf((char*)tmp.c_str(), "%d", hp2);
						shp2 = tmp.c_str();
						text2.setString(shp2);
					}
					else if(sprite6.getGlobalBounds().contains(xx,yy))
					{
						hp1+=5;
						tmp="";
						sprintf((char*)tmp.c_str(), "%d", hp1);
						shp1 = tmp.c_str();
						text1.setString(shp1);

						psa=2;
						sprite8.move(Vector2f(35,0));
					}
				}

			}
			//wyslanie i odbieranie pakietow
			Packet pck2;
			if(psa!=0){
				pck2<<psa;
				gsck2.send(pck2);
			}

			gsck2.receive(pck2);
			if(pck2>>pca){
				if(pca==1)
					sprite7.move(Vector2f(-35,0));
				else if(pca==2)
					sprite8.move(Vector2f(-35,0));
			}
			
		}
		window.clear(sf::Color::Blue);
		window.draw(sprite3);
		window.draw(sprite4);
	//	window.draw(sprite5);
		window.draw(sprite6);
		window.draw(sprite7);
		window.draw(sprite8);
		window.draw(sprite1);
		window.draw(sprite2);
		
		window.draw(text1);
		window.draw(text2);
		
        window.display();
    }
	}
	


	/////////////////////////
	//Glowna petla dla klienta
	/////////////////////////
	else{
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

			pcp=sprite2.getPosition();

			//ruch postaci
			if(event.type==Event::KeyPressed)
			{
				if(event.key.code==Keyboard::W)
					sprite2.move(Vector2f(0,-35));
				else if(event.key.code==Keyboard::S)
					sprite2.move(Vector2f(0,35));
				else if(event.key.code==Keyboard::A)
					sprite2.move(Vector2f(-35,0));
				else if(event.key.code==Keyboard::D)
					sprite2.move(Vector2f(35,0));
			}

			Packet pck;
			if(pcp!=sprite2.getPosition()){
				pck<<sprite2.getPosition().x<<sprite2.getPosition().y;
				gsck.send(pck);
			}

			gsck.receive(pck);
			if(pck>>psp.x>>psp.y){
				sprite1.setPosition(psp);
			}
			pca=0;
			//klikanie w guziki odpowiadajace atakowi, obronie etc
			if(event.type == Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					Vector2i x=Mouse::getPosition(window);
					float xx=(float)x.x;
					float yy=(float)x.y;
					if(sprite4.getGlobalBounds().contains(xx,yy))
					{
						hp1+=5;
						tmp="";
						sprintf((char*)tmp.c_str(), "%d", hp1);
						shp1 = tmp.c_str();
						text1.setString(shp1);

						sprite8.move(Vector2f(-35,0));
						pca=1;
					}
					else if(sprite6.getGlobalBounds().contains(xx,yy))
					{
						hp2-=5;
						tmp="";
						sprintf((char*)tmp.c_str(), "%d", hp2);
						shp2 = tmp.c_str();
						text2.setString(shp2);

						sprite7.move(Vector2f(-35,0));
						pca=2;
					}
				}
			}

			
			//wyslanie i odbieranie pakietow
			Packet pck2;
			if(pca!=0){
				pck2<<pca;
				gsck2.send(pck2);
			}

			gsck2.receive(pck2);
			if(pck2>>pca){
				if(pca==1)
					sprite8.move(Vector2f(35,0));
				else if(pca==2)
					sprite7.move(Vector2f(35,0));
			}
        }

		window.clear(sf::Color::Blue);
		window.draw(sprite3);
		window.draw(sprite4);
		//window.draw(sprite5);
		window.draw(sprite6);
		window.draw(sprite7);
		window.draw(sprite8);
		window.draw(sprite1);
		window.draw(sprite2);
		
		window.draw(text1);
		window.draw(text2);
		
        window.display();
    }
	};
    return 0;
}