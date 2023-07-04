#include<stdio.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_native_dialog.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_image.h>
#include<allegro5/file.h>

#define ANCHO 1600
#define ALTO 800
#define ANCHOPERSONAJE 50
#define ALTOPERSONAJE 58
#define x_pixel 41
#define y_pixel 20

struct _personaje
{
	int x;
	int y;
	float vel;
	int madera_cant;
	int roca_cant;
};
typedef struct _personaje personaje;
personaje obrero = { ANCHO / 2 - ANCHOPERSONAJE / 2, ALTO / 2 - ALTOPERSONAJE / 2, 4.5 };

/*
struct _arbol
{
	int cantidad = 10;
};
typedef struct _arbol arbol;

struct _roca
{
	int cantidad = 10;
};
typedef struct _roca roca;

struct _muro
{

};
typedef struct _muro muro;

struct _cesped
{

};
typedef struct _cesped cesped;

struct _cesped2
{

};
typedef struct _cesped2 cesped2;

struct _camino
{

};
typedef struct _camino camino;

struct _camino2
{

};
typedef struct _camino2 camino2;
*/

int tecla_up = false;
int tecla_down = false;
int tecla_left = false;
int tecla_right = false;

bool ejecutandose = true;

//Funci�n encargada del teclado cuando se presiona una tecla.

void proceso_teclado_input(ALLEGRO_EVENT juego)
{
	switch (juego.keyboard.keycode)
	{
		case ALLEGRO_KEY_UP: tecla_up = juego.type = ALLEGRO_EVENT_KEY_DOWN;
		break;
		case ALLEGRO_KEY_DOWN: tecla_down = juego.type = ALLEGRO_EVENT_KEY_DOWN;
		break;
		case ALLEGRO_KEY_LEFT: tecla_left = juego.type = ALLEGRO_EVENT_KEY_DOWN;
		break;
		case ALLEGRO_KEY_RIGHT: tecla_right = juego.type = ALLEGRO_EVENT_KEY_DOWN;
		break;
		case ALLEGRO_KEY_ESCAPE: ejecutandose = false;
		break;
	}
}

//Funci�n encargada de teclado cuando se deja de pulsar una tecla.

void proceso_teclado_output(ALLEGRO_EVENT juego)
{
	switch (juego.keyboard.keycode)
	{
		case ALLEGRO_KEY_UP: tecla_up = false;
		break;
		case ALLEGRO_KEY_DOWN: tecla_down = false;
		break;
		case ALLEGRO_KEY_LEFT: tecla_left = false;
		break;
		case ALLEGRO_KEY_RIGHT: tecla_right = false;
		break;
	}
}

//Funci�n encargada de la colisi�n.

void colision()
{
	if (tecla_up && obrero.y > 0)
	{
		obrero.y -= obrero.vel;
	}
	if (tecla_down && obrero.y + ALTOPERSONAJE < ALTO)
	{
		obrero.y += obrero.vel;
	}
	if (tecla_left && obrero.x > 0)
	{
		obrero.x -= obrero.vel;
	}
	if (tecla_right && obrero.x + ANCHOPERSONAJE < ANCHO)
	{
		obrero.x += obrero.vel;
	}
}

int main()
{
	//Inicializaciones.

	ALLEGRO_DISPLAY* ventana = NULL;
	ALLEGRO_EVENT_QUEUE* eventos = NULL;
	ALLEGRO_TIMER* fps = NULL;
	ALLEGRO_BITMAP* obrero1 = NULL;
	ALLEGRO_BITMAP* fondo = NULL;
	ALLEGRO_BITMAP* muro = NULL;
	ALLEGRO_BITMAP* camino = NULL;
	ALLEGRO_BITMAP* camino2 = NULL;
	ALLEGRO_BITMAP* cesped = NULL;
	ALLEGRO_BITMAP* cesped2 = NULL;
	ALLEGRO_BITMAP* arbol = NULL;
	ALLEGRO_BITMAP* roca = NULL;
	ALLEGRO_FILE* mapa = al_fopen("mapa.txt", "r");

	int x_mapa, y_mapa;
	char archivo[y_pixel][x_pixel];

	//Fin inicializaciones.

	//Errores posibles e inicializaciones.

	if (!al_init())
	{
		printf("Error al iniciar allegro.h");
		return -1;
	}

	ventana = al_create_display(ANCHO, ALTO);
	if (!ventana)
	{
		printf("Error al abrir la ventana");
		al_destroy_display(ventana);
		return -1;
	}
	
	al_install_keyboard();
	if (!al_install_keyboard)
	{
		printf("Error al instalar teclado");
		al_destroy_display(ventana);
		return -1;
	}

	eventos = al_create_event_queue();
	if (!eventos)
	{
		printf("Error al crear cola de eventos");
		al_destroy_display(ventana);
		return -1;
	}

	fps = al_create_timer(1.0 / 60);
	if (!fps)
	{
		printf("Error al crear temporizador");
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}

	al_init_image_addon();
	obrero1 = al_load_bitmap("obrero.png");
	if (!obrero1)
	{
		printf("Error al cargar el obrero 1");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	fondo = al_load_bitmap("fondo.png");
	if (!fondo)
	{
		printf("Error al cargar el fondo del mapa");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	muro = al_load_bitmap("muro.png");
	if (!muro)
	{
		printf("Error al cargar un pixel de muro");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	camino = al_load_bitmap("camino.png");
	if (!camino)
	{
		printf("Error al cargar un pixel de camino");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	camino2 = al_load_bitmap("camino2.png");
	if (!camino)
	{
		printf("Error al cargar un pixel de camino2");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	cesped = al_load_bitmap("cesped.png");
	if (!cesped)
	{
		printf("Error al cargar un pixel de cesped");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	cesped2 = al_load_bitmap("cesped2.png");
	if (!cesped2)
	{
		printf("Error al cargar un pixel de cesped2");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	arbol = al_load_bitmap("arbol.png");
	if (!arbol)
	{
		printf("Error al cargar un pixel de arbol");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	roca = al_load_bitmap("roca.png");
	if (!roca)
	{
		printf("Error al cargar un pixel de roca");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}

	al_register_event_source(eventos, al_get_display_event_source(ventana));
	al_register_event_source(eventos, al_get_timer_event_source(fps));
	al_register_event_source(eventos, al_get_keyboard_event_source());

	al_set_window_title(ventana, "Protect the CORE");
	al_flip_display();
	al_start_timer(fps);

	//Fin errores posibles e inicializaciones.

	//Movimiento y colisiones, juego ejecutandose.

	for (y_mapa = 0; y_mapa < y_pixel; y_mapa++)
	{
		for (x_mapa = 0; x_mapa < x_pixel; x_mapa++)
		{
			archivo[y_mapa][x_mapa] = al_fgetc(mapa);
		}
	}
	al_fclose(mapa);

	while (ejecutandose)
	{
		ALLEGRO_EVENT juego;
		al_wait_for_event(eventos, &juego);
		if (juego.type == ALLEGRO_EVENT_TIMER)
		{
			colision();
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(fondo, 0, 0, 0);
			for (y_mapa = 0; y_mapa < y_pixel; y_mapa++)
			{
				for (x_mapa = 0; x_mapa < x_pixel; x_mapa++)
				{
					if (archivo[y_mapa][x_mapa] == 'm')
					{
						al_draw_bitmap(muro, x_mapa * 40, y_mapa * 40, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 't')
					{
						al_draw_bitmap(camino, x_mapa * 40, y_mapa * 40, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 'k')
					{
						al_draw_bitmap(camino2, x_mapa * 40, y_mapa * 40, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 'c')
					{
						al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 's')
					{
						al_draw_bitmap(cesped2, x_mapa * 40, y_mapa * 40, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 'a')
					{
						al_draw_bitmap(arbol, x_mapa * 40, y_mapa * 40, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 'r')
					{
						al_draw_bitmap(roca, x_mapa * 20, y_mapa * 20, 0);
					}
				}
			}
			al_draw_bitmap(obrero1, obrero.x, obrero.y, 0);
			al_flip_display();
		}
		else if (juego.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			ejecutandose = false;
		}
		else if (juego.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			proceso_teclado_input(juego);
		}
		else if (juego.type == ALLEGRO_EVENT_KEY_UP)
		{
			proceso_teclado_output(juego);
		}
	}

	//Fin de movimiento y colisiones, fin de la ejecuci�n del juego.

	//Liberaci�n de memoria.

	al_destroy_bitmap(fondo);
	al_destroy_bitmap(obrero1);
	al_destroy_bitmap(muro);
	al_destroy_bitmap(camino);
	al_destroy_bitmap(cesped);
	al_destroy_timer(fps);
	al_destroy_event_queue(eventos);
	al_destroy_display(ventana);

	//Fin de la liberaci�n de memoria.

	return 0;
}