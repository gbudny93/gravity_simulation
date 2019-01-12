/*------------------------------------------------------------------------
--------------------------GRACVITY SIMULATION-----------------------------
...................Grzegorz Budny CP Project II 2014----------------------
------------------------------------------------------------------------*/

#define G	6.6e-11	//Gravity constant
//Libraries
#include<stdio.h> //standard input output library
#include<allegro5\allegro.h> //allegro library
#include<allegro5\allegro_primitives.h> //allegro library
#include<math.h> //library that posses mathematical functions

//Global variables
//Allegro variables
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT event;
bool loop = true;

//Class that posses all data and methods about a planet
class Planet
{
	
public:
	float x;		//position of the planet on x axis
	float y;		//position of the planet on y axis
	float angle;	//angle between moving vectors of the plaet
	float radius;	//distance from the planet to the sun
	float vx;		//velocity of the planet through x axis		
	float vy;		//velocity of the planet through y axis
	float mass;		//mass of the planet
	float a;		//acceleration of the planet
	float z;		//size of the drawn planet
	int r, g, b;	//colors of the planet

	//contructor that creates a planet with initialization list
	Planet(float x, float y, float vx, float vy, float mass, float z, int r,int g, int b) : x(x), y(y), vx(vx), vy(vy), mass(mass),
		angle(0), radius(0), a(0), r(r), g(g), b(b), z(z){}

	//function that draw created planet in the world
	void draw()
	{
		al_draw_filled_circle(float(x / 10000) + 320, float(y / 10000) + 240,
			z, al_map_rgb(r, g, b));
	}

	//function that updates planets' coordinates
	void update()
	{
		radius = sqrt(pow(x, 2) + pow(y, 2)); //calculate distance
		a = (G*(mass / pow(radius, 2)));//calculate acceleration
		angle = atan2f(y, x);//calculate angle
		vx -= (cos(angle)*a); //update velocity
		vy -= (sin(angle)*a); //update velocity
		x += vx; //update position x
		y += vy; //update position y
	}
};
//class that posses data about the star
class Sun : Planet
{
public:
	Sun() : Planet(0,0,0,0,0,10,250,235,22){}//constructor that creates a star
	//function that draw the star
	void draw() {
		al_draw_filled_circle(640 / 2, 480 / 2, z, al_map_rgb(r, g, b));
	}
	//function that updates stars' coordinates 
	void update() {}
};
//class that create a universe and put created planet into it
class World
{
public:
	//creating objects for new planets and a sun
	Planet* p1; 
	Planet* p2;
	Sun* sun;
	//Constructor that allocates memory and creates planets and star
	World()
	{
		sun = new Sun(); 
		p1 = new Planet(2.4e6, 7.5e5, 0, 8.5e3, 5.9e24,7,55,156,193);
		p2 = new Planet(1e6, 3e5, 0, 9e3, 0.81*5.9e24,4,174,144,144);
	}
	//Destructor that frees memory and destroys planets and sun
	~World()
	{
		delete p1;
		delete p2;
		delete sun;
	}
	//function that creates a world 
	void Create_World()
	{

		display = al_create_display(640, 480); //creates display
		al_init_primitives_addon();//primitives initialization
		timer = al_create_timer(1.0f / 60);//fps

	}
	//function that handle events
	void Handle_Event(ALLEGRO_EVENT &event)
	{
		switch (event.type)
		{
			case ALLEGRO_EVENT_DISPLAY_CLOSE: //if display is closed the program stops
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN://if the mouse button is pushed the program stops
				loop = false; //while loop condition
				break;

			case ALLEGRO_EVENT_TIMER: //when the timer is active
				update(); //update coordinates 
				draw(); //draw the planets
				break;
		}

	}
	//function that updates positions of planets
	void update() {
		p1->update();//updates planet p1
		p2->update();//updates planet p2
	}
	//function that draw created planets in created world
	void draw()
	{
		al_clear_to_color(al_map_rgb(0, 0, 0));//refreshing the screen
		sun->draw();//draws sun
		p1->draw();//draws planet p1
		p2->draw();//draws planet p1
		al_flip_display();
	}


};

int main(){

	
	//initialization of display, if it fails a proper comment is displayed on the screen
	if (!al_init()){
		printf("display initialization failed");
		return -1;
	}

	World world;//create new object world
	world.Create_World();//calling a method Create_World()

	//queue of events
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);

	//a loop where all events are handled and execute one by one
	while (loop)
	{
		al_wait_for_event(event_queue, &event); //waiting for an event
		world.Handle_Event(event); //handle event and execute it
	}

	al_stop_timer(timer); //stops the timer
	al_destroy_timer(timer); //destroys timer
	al_destroy_display(display); //destroys display
	al_destroy_event_queue(event_queue); //destroys event queue
	
	return 0;

}