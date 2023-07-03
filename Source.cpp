#include<stdio.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_native_dialog.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_image.h>
#include<allegro5/file.h>

#define ANCHO 1600
#define ALTO 900
#define ANCHOPERSONAJE 50
#define ALTOPERSONAJE 58
#define VELOCIDAD_PERSONAJE 3
#define x_pixel 41
#define y_pixel 31

int x_personaje = ANCHO / 2 - ANCHOPERSONAJE / 2;
int y_personaje = ALTO / 2 - ALTOPERSONAJE / 2;

int tecla_up = false;
int tecla_down = false;
int tecla_left = false;
int tecla_right = false;

bool ejecutandose = true;

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

void colision()
{
	if (tecla_up && y_personaje > 0)
	{
		y_personaje -= VELOCIDAD_PERSONAJE;
	}
	if (tecla_down && y_personaje + ALTOPERSONAJE < ALTO)
	{
		y_personaje += VELOCIDAD_PERSONAJE;
	}
	if (tecla_left && x_personaje > 0)
	{
		x_personaje -= VELOCIDAD_PERSONAJE;
	}
	if (tecla_right && x_personaje + ANCHOPERSONAJE < ANCHO)
	{
		x_personaje += VELOCIDAD_PERSONAJE;
	}
}

int main()
{
	ALLEGRO_DISPLAY* ventana = NULL;
	ALLEGRO_EVENT_QUEUE* eventos = NULL;
	ALLEGRO_TIMER* fps = NULL;
	ALLEGRO_BITMAP* obrero = NULL;
	ALLEGRO_BITMAP* fondo = NULL;
	ALLEGRO_BITMAP* muro = NULL;
	ALLEGRO_BITMAP* camino = NULL;
	ALLEGRO_BITMAP* cesped = NULL;
	ALLEGRO_BITMAP* arbol = NULL;
	ALLEGRO_BITMAP* roca = NULL;
	ALLEGRO_FILE* mapa = al_fopen("mapa.txt", "r");

	int x_mapa, y_mapa;
	char archivo[y_pixel][x_pixel];

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
	obrero = al_load_bitmap("obrero.png");
	if (!obrero)
	{
		printf("Error al cargar obrero");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	fondo = al_load_bitmap("fondo.jpg");
	if (!fondo)
	{
		printf("Error al cargar el fondo");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	muro = al_load_bitmap("muro.jpg");
	if (!muro)
	{
		printf("Error al cargar el muro");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	camino = al_load_bitmap("camino.jpg");
	if (!camino)
	{
		printf("Error al cargar el camino");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	cesped = al_load_bitmap("cesped.jpg");
	if (!cesped)
	{
		printf("Error al cargar el cesped");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	arbol = al_load_bitmap("arbol.png");
	if (!arbol)
	{
		printf("Error al cargar el arbol");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	roca = al_load_bitmap("roca.png");
	if (!roca)
	{
		printf("Error al cargar la roca");
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
						al_draw_bitmap(muro, x_mapa * 20, y_mapa * 20, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 't')
					{
						al_draw_bitmap(camino, x_mapa * 20, y_mapa * 20, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 'c')
					{
						al_draw_bitmap(cesped, x_mapa * 20, y_mapa * 20, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 'a')
					{
						al_draw_bitmap(arbol, x_mapa * 20, y_mapa * 20, 0);
					}
					else if (archivo[y_mapa][x_mapa] == 'r')
					{
						al_draw_bitmap(roca, x_mapa * 20, y_mapa * 20, 0);
					}
				}
			}
			al_draw_bitmap(obrero, x_personaje, y_personaje, 0);
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

	//Fin de movimiento y colisiones, fin de la ejecución del juego.

	//Liberación de memoria.

	al_destroy_bitmap(fondo);
	al_destroy_bitmap(obrero);
	al_destroy_bitmap(muro);
	al_destroy_bitmap(camino);
	al_destroy_bitmap(cesped);
	al_destroy_timer(fps);
	al_destroy_event_queue(eventos);
	al_destroy_display(ventana);

	//Fin de la liberación de memoria.

	return 0;
}