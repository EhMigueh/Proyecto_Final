//Bibliotecas.

#include<stdio.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_native_dialog.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_image.h>
#include<allegro5/file.h>

//Defines.

#define ANCHO 1600
#define ALTO 800
#define ANCHOPERSONAJE 50
#define ALTOPERSONAJE 58
#define x_pixel 41
#define y_pixel 20

//Variables globales.

int cantmadera = 0;
int cantroca = 0;
int x_mapa;
int y_mapa;
int x_act;
int y_act;
int x_sig;
int y_sig;
int tecla_up = false;
int tecla_down = false;
int tecla_left = false;
int tecla_right = false;
char archivo[y_pixel][x_pixel];
bool ejecutandose = true;

//Estructuras.

struct _personaje
{
	int x;
	int y;
	float vel;
};
typedef struct _personaje personaje;
personaje obrero = { ANCHO / 2 - ANCHOPERSONAJE / 2, ALTO / 2 - ALTOPERSONAJE / 2, 2.5 };
struct _arbol
{
	int cantidad = 20;
};
typedef struct _arbol arbol;
struct _roca
{
	int cantidad = 20;
};
typedef struct _roca roca;

//Función encargada del teclado cuando se presiona una tecla.

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

//Función encargada de teclado cuando se deja de pulsar una tecla.

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

//Funcion encargada de la colision con los bordes.

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

//Función principal.

int main()
{
	//Inicializaciones

	ALLEGRO_DISPLAY* ventana = NULL;
	ALLEGRO_EVENT_QUEUE* eventos = NULL;
	ALLEGRO_TIMER* fps = NULL;
	ALLEGRO_BITMAP* obrero1 = NULL;
	ALLEGRO_BITMAP* muro = NULL;
	ALLEGRO_BITMAP* camino = NULL;
	ALLEGRO_BITMAP* cespedycamino = NULL;
	ALLEGRO_BITMAP* cespedycamino2 = NULL;
	ALLEGRO_BITMAP* cesped = NULL;
	ALLEGRO_BITMAP* arbol = NULL;
	ALLEGRO_BITMAP* roca = NULL;
	ALLEGRO_BITMAP* portal= NULL;
	ALLEGRO_BITMAP* nucleo = NULL;
	ALLEGRO_FILE* mapa = al_fopen("mapa.txt", "r");

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
	cespedycamino = al_load_bitmap("cespedycamino.png");
	if (!cespedycamino)
	{
		printf("Error al cargar un pixel de cesped y camino");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	cespedycamino2 = al_load_bitmap("cespedycamino2.png");
	if (!cespedycamino2)
	{
		printf("Error al cargar un pixel de cesped y camino 2");
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
	portal = al_load_bitmap("portal.png");
	if (!portal)
	{
		printf("Error al cargar un pixel de portal");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	nucleo = al_load_bitmap("nucleo.png");
	if (!nucleo)
	{
		printf("Error al cargar un pixel de nucleo");
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

	//Lector de mapa.

	for (y_mapa = 0; y_mapa < y_pixel; y_mapa++)
	{
		for (x_mapa = 0; x_mapa < x_pixel; x_mapa++)
		{
			archivo[y_mapa][x_mapa] = al_fgetc(mapa);
		}
	}
	al_fclose(mapa);

	//Juego ejecutandose.

	while (ejecutandose)
	{
		ALLEGRO_EVENT juego;
		al_wait_for_event(eventos, &juego);
		if (juego.type == ALLEGRO_EVENT_TIMER)
		{
			colision();
			al_clear_to_color(al_map_rgb(0, 0, 0));
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
						al_draw_bitmap(cespedycamino, x_mapa * 40, y_mapa * 40, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 'l')
					{
						al_draw_bitmap(cespedycamino2, x_mapa * 40, y_mapa * 40, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 'c')
					{
						al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 'a')
					{
						al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
						al_draw_bitmap(arbol, x_mapa * 40, y_mapa * 40, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 'r')
					{
						al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
						al_draw_bitmap(roca, x_mapa * 40, y_mapa * 40, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 'p')
					{
						al_draw_bitmap(camino, x_mapa * 40, y_mapa * 40, 0);
						al_draw_bitmap(portal, x_mapa * 40, y_mapa * 40, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 'n')
					{
						al_draw_bitmap(camino, x_mapa * 40, y_mapa * 40, 0);
						al_draw_bitmap(nucleo, x_mapa * 40, y_mapa * 40, 0);
					}
				}
			}
			if (tecla_up && obrero.y > 0)
			{
				y_sig = (obrero.y - obrero.vel) / 40;
				x_act = obrero.x / 40;
				if (archivo[y_sig][x_act] != 'm' && archivo[y_sig][x_act] != 'a' && archivo[y_sig][x_act] != 'r' && archivo[y_sig][x_act] != 'p' && archivo[y_sig][x_act] != 'n' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'm' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'a' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'r' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'p' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'n')
				{
					obrero.y -= obrero.vel;
				}
				else
				{
					obrero.y = (y_sig + 1) * 40;
				}
			}
			if (tecla_down && obrero.y + ALTOPERSONAJE < ALTO)
			{
				y_sig = (obrero.y + obrero.vel + ALTOPERSONAJE) / 40;
				x_act = obrero.x / 40;
				if (archivo[y_sig][x_act] != 'm' && archivo[y_sig][x_act] != 'a' && archivo[y_sig][x_act] != 'r' && archivo[y_sig][x_act] != 'p' && archivo[y_sig][x_act] != 'n' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'm' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'a' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'r' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'p' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'n')
				{
					obrero.y += obrero.vel;
				}
				else
				{
					obrero.y = y_sig * 40 - ALTOPERSONAJE;
				}
			}
			if (tecla_left && obrero.x > 0)
			{
				y_act = obrero.y / 40;
				x_sig = (obrero.x - obrero.vel) / 40;
				if (archivo[y_act][x_sig] != 'm' && archivo[y_act][x_sig] != 'a' && archivo[y_act][x_sig] != 'r' && archivo[y_act][x_sig] != 'p' && archivo[y_act][x_sig] != 'n' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'm' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'a' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'r' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'p' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'n')
				{
					obrero.x -= obrero.vel;
				}
				else
				{
					obrero.x = (x_sig + 1) * 40;
				}
			}
			if (tecla_right && obrero.x + ANCHOPERSONAJE < ANCHO)
			{
				y_act = obrero.y / 40;
				x_sig = (obrero.x + obrero.vel + ANCHOPERSONAJE) / 40;
				if (archivo[y_act][x_sig] != 'm' && archivo[y_act][x_sig] != 'a' && archivo[y_act][x_sig] != 'r' && archivo[y_act][x_sig] != 'p' && archivo[y_act][x_sig] != 'n' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'm' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'a' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'r' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'p' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'n')
				{
					obrero.x += obrero.vel;
				}
				else
				{
					obrero.x = x_sig * 40 - ANCHOPERSONAJE;
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

	//Liberación de memoria.

	al_destroy_bitmap(obrero1);
	al_destroy_bitmap(muro);
	al_destroy_bitmap(camino);
	al_destroy_bitmap(cespedycamino);
	al_destroy_bitmap(cespedycamino2);
	al_destroy_bitmap(cesped);
	al_destroy_bitmap(arbol);
	al_destroy_bitmap(roca);
	al_destroy_bitmap(portal);
	al_destroy_bitmap(nucleo);
	al_destroy_timer(fps);
	al_destroy_event_queue(eventos);
	al_destroy_display(ventana);

	return 0;
}