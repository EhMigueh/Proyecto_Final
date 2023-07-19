//Bibliotecas.
#include<time.h>
#include<stdio.h>
#include<allegro5/file.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_native_dialog.h>
//Bibliotecas.

//Valores definidos.
#define ANCHO 1600
#define ALTO 800
#define ANCHOPERSONAJE 32
#define ALTOPERSONAJE 32
#define x_pixel 41
#define y_pixel 20
//Valores definidos.

//Variables globales.
int tecla_up = false;
int tecla_down = false;
int tecla_left = false;
int tecla_right = false;
int contpiedra = 0;
int contmadera = 0;
bool ejecutandose = true;
time_t tiempo_ultima_recoleccion = 0;
//Variables globales.

//Estructuras.
struct _personaje
{
	int x;
	int y;
	float vel;
	bool direccion;
};
typedef struct _personaje personaje;
personaje obrero = { ANCHO / 2 - ANCHOPERSONAJE / 2, ALTO / 2 - ALTOPERSONAJE / 2, 2, false};
struct _enemigo
{
	int vida;
	int x;
	int y;
	bool activo;
};
typedef struct _enemigo enemigo;
//Estructuras.

//Función encargada del teclado cuando se presiona una tecla.
void proceso_teclado_input(ALLEGRO_EVENT juego)
{
	switch (juego.keyboard.keycode)
	{
	case ALLEGRO_KEY_UP: tecla_up = true;
		break;
	case ALLEGRO_KEY_DOWN: tecla_down = true;
		break;
	case ALLEGRO_KEY_LEFT: tecla_left = true;
		break;
	case ALLEGRO_KEY_RIGHT: tecla_right = true;
		break;
	case ALLEGRO_KEY_ESCAPE: ejecutandose = false;
		break;
	}
}
//Función encargada del teclado cuando se presiona una tecla.

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
//Función encargada de teclado cuando se deja de pulsar una tecla.

//Función encargada de la colision con los bordes.
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
		obrero.direccion = false;
	}
	if (tecla_right && obrero.x + ANCHOPERSONAJE < ANCHO)
	{
		obrero.x += obrero.vel;
		obrero.direccion = true;
	}
}
//Función encargada de la colision con los bordes.

//Función para dibujar objetos.
void dibujar(char archivo[y_pixel][x_pixel], ALLEGRO_BITMAP* muro, ALLEGRO_BITMAP* camino, ALLEGRO_BITMAP* cespedycamino, ALLEGRO_BITMAP* cespedycamino2, ALLEGRO_BITMAP* cesped, ALLEGRO_BITMAP* arbol, ALLEGRO_BITMAP* roca, ALLEGRO_BITMAP* nucleo, ALLEGRO_BITMAP* nucleo_borde)
{
	int x_mapa;
	int y_mapa;
	int contbosque = 0;
	int controca = 0;

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
			else if (archivo[y_mapa][x_mapa] == 'n')
			{
				al_draw_bitmap(camino, x_mapa * 40, y_mapa * 40, 0);
				al_draw_bitmap(nucleo, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'b')
			{
				al_draw_bitmap(camino, x_mapa * 40, y_mapa * 40, 0);
				al_draw_bitmap(nucleo_borde, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'o')
			{
				al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
			}
		}
	}
}
//Función para dibujar objetos.

//Recolector y contador piedra.
void recolectar_piedra(char archivo[y_pixel][x_pixel])
{
	int x_mapa;
	int y_mapa;
	time_t tiempo_actual;

	x_mapa = obrero.x / 40;
	y_mapa = obrero.y / 40;
	if (archivo[y_mapa][x_mapa] == 'r')
	{
		tiempo_actual = time(NULL);
		if (tiempo_actual - tiempo_ultima_recoleccion >= 3)
		{
			archivo[y_mapa][x_mapa] = 'o';
			contpiedra++;
			tiempo_ultima_recoleccion = tiempo_actual;
		}
	}
	if (archivo[y_mapa][(obrero.x + ANCHOPERSONAJE - 1) / 40] == 'r')
	{
		tiempo_actual = time(NULL);
		if (tiempo_actual - tiempo_ultima_recoleccion >= 3)
		{
			archivo[y_mapa][(obrero.x + ANCHOPERSONAJE - 1) / 40] = 'o';
			contpiedra++;
			tiempo_ultima_recoleccion = tiempo_actual;
		}
	}
	else if (archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_mapa] == 'r')
	{
		tiempo_actual = time(NULL);
		if (tiempo_actual - tiempo_ultima_recoleccion >= 3)
		{
			archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_mapa] = 'o';
			contpiedra++;
			tiempo_ultima_recoleccion = tiempo_actual;
		}
	}
	else if (archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][(obrero.x + ANCHOPERSONAJE - 1) / 40] == 'r')
	{
		tiempo_actual = time(NULL);
		if (tiempo_actual - tiempo_ultima_recoleccion >= 3)
		{
			archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][(obrero.x + ANCHOPERSONAJE - 1) / 40] = 'o';
			contpiedra++;
			tiempo_ultima_recoleccion = tiempo_actual;
		}
	}
}
//Recolector y contador piedra.

//Recolector y contador madera.
void recolectar_madera(char archivo[y_pixel][x_pixel])
{
	int x_mapa;
	int y_mapa;
	time_t tiempo_actual;

	x_mapa = obrero.x / 40;
	y_mapa = obrero.y / 40;
	if (archivo[y_mapa][x_mapa] == 'a')
	{
		tiempo_actual = time(NULL);
		if (tiempo_actual - tiempo_ultima_recoleccion >= 3)
		{
			archivo[y_mapa][x_mapa] = 'o';
			contmadera++;
			tiempo_ultima_recoleccion = tiempo_actual;
		}
	}
	else if (archivo[y_mapa][(obrero.x + ANCHOPERSONAJE - 1) / 40] == 'a')
	{
		tiempo_actual = time(NULL);
		if (tiempo_actual - tiempo_ultima_recoleccion >= 3)
		{
			archivo[y_mapa][(obrero.x + ANCHOPERSONAJE - 1) / 40] = 'o';
			contmadera++;
			tiempo_ultima_recoleccion = tiempo_actual;
		}
	}
	else if (archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_mapa] == 'a')
	{
		tiempo_actual = time(NULL);
		if (tiempo_actual - tiempo_ultima_recoleccion >= 3)
		{
			archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_mapa] = 'o';
			contmadera++;
			tiempo_ultima_recoleccion = tiempo_actual;
		}
	}
	else if (archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][(obrero.x + ANCHOPERSONAJE - 1) / 40] == 'a')
	{
		tiempo_actual = time(NULL);
		if (tiempo_actual - tiempo_ultima_recoleccion >= 3)
		{
			archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][(obrero.x + ANCHOPERSONAJE - 1) / 40] = 'o';
			contmadera++;
			tiempo_ultima_recoleccion = tiempo_actual;
		}
	}
}
//Recolector y contador madera.

//Función principal.
int main()
{
	//Declaraciones de ALLEGRO.
	ALLEGRO_DISPLAY* ventana = NULL;
	ALLEGRO_EVENT_QUEUE* eventos = NULL;
	ALLEGRO_TIMER* fps = NULL;
	ALLEGRO_BITMAP* obrero_right = NULL;
	ALLEGRO_BITMAP* obrero_left = NULL;
	ALLEGRO_BITMAP* obrero_up = NULL;
	ALLEGRO_BITMAP* obrero_down = NULL;
	ALLEGRO_BITMAP* muro = NULL;
	ALLEGRO_BITMAP* camino = NULL;
	ALLEGRO_BITMAP* cespedycamino = NULL;
	ALLEGRO_BITMAP* cespedycamino2 = NULL;
	ALLEGRO_BITMAP* cesped = NULL;
	ALLEGRO_BITMAP* arbol = NULL;
	ALLEGRO_BITMAP* roca = NULL;
	ALLEGRO_BITMAP* nucleo = NULL;
	ALLEGRO_BITMAP* nucleo_borde = NULL;
	ALLEGRO_FONT* font_pos = NULL;
	ALLEGRO_FONT* font_piedra = NULL;
	ALLEGRO_FONT* font_madera = NULL;
	ALLEGRO_FILE* mapa = al_fopen("mapa.txt", "r");
	//Declaraciones de ALLEGRO.

	//Declaración variables locacles función principal.
	int x_act;
	int y_act;
	int x_sig;
	int y_sig;
	int x_mapa;
	int y_mapa;
	char archivo[y_pixel][x_pixel];
	char posicion_texto[50];
	char conpiedra_texto[50];
	char conmadera_texto[50];
	tiempo_ultima_recoleccion = time(NULL);
	//Declaración variables locacles función principal.

	//Errores e inicializaciones de ALLEGRO.
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
	obrero_right = al_load_bitmap("obrero_right.png");
	if (!obrero_right)
	{
		printf("Error al cargar el imagen obrero mirando derecha");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	obrero_left = al_load_bitmap("obrero_left.png");
	if (!obrero_left)
	{
		printf("Error al cargar el imagen obrero mirando izquierda");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	obrero_up = al_load_bitmap("obrero_up.png");
	if (!obrero_up)
	{
		printf("Error al cargar el imagen obrero mirando arriba");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	obrero_down = al_load_bitmap("obrero_down.png");
	if (!obrero_down)
	{
		printf("Error al cargar el imagen obrero mirando abajo");
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
	nucleo = al_load_bitmap("nucleo.png");
	if (!nucleo)
	{
		printf("Error al cargar un pixel de núcleo");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	nucleo_borde = al_load_bitmap("nucleo_borde.png");
	if (!nucleo_borde)
	{
		printf("Error al cargar un pixel de borde de núcleo");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	al_init_font_addon();
	al_init_ttf_addon();
	font_pos = al_load_font("arial.ttf", 20, 0);
	if (!font_pos)
	{
		printf("Error al cargar la fuente de posición");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	font_piedra = al_load_font("arial.ttf", 20, 0);
	if (!font_piedra)
	{
		printf("Error al cargar la fuente de piedra");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	font_madera = al_load_font("arial.ttf", 20, 0);
	if (!font_madera)
	{
		printf("Error al cargar la fuente de madera");
		al_destroy_timer(fps);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	//Errores e inicializaciones de ALLEGRO.

	//Registro de eventos.
	al_register_event_source(eventos, al_get_display_event_source(ventana));
	al_register_event_source(eventos, al_get_timer_event_source(fps));
	al_register_event_source(eventos, al_get_keyboard_event_source());
	al_set_window_title(ventana, "Protect the CORE");
	al_flip_display();
	al_start_timer(fps);
	//Registro de eventos.

	//Lector de mapa.
	for (y_mapa = 0; y_mapa < y_pixel; y_mapa++)
	{
		for (x_mapa = 0; x_mapa < x_pixel; x_mapa++)
		{
			archivo[y_mapa][x_mapa] = al_fgetc(mapa);
		}
	}
	al_fclose(mapa);
	//Lector de mapa.

	//Ejecución del juego.
	while (ejecutandose)
	{
		//Evento juego.
		ALLEGRO_EVENT juego;
		al_wait_for_event(eventos, &juego);
		//Evento juego.

		//Cuerpo juego.
		if (juego.type == ALLEGRO_EVENT_TIMER)
		{
			//Llamada función de colisión con bordes.
			colision();
			//Llamada función de colisión con bordes.

			//Limpieza de los dibujos.
			al_clear_to_color(al_map_rgb(0, 0, 0));
			//Limpieza de los dibujos.

			//Dibujo objetos.
			dibujar(archivo, muro, camino, cespedycamino, cespedycamino2, cesped, arbol, roca, nucleo, nucleo_borde);
			//Dibujo objetos.

			//Dibujo obrero.
			if (obrero.direccion)
			{
				if (tecla_up)
				{
					al_draw_bitmap(obrero_up, obrero.x, obrero.y, 0);
				}
				else if (tecla_down)
				{
					al_draw_bitmap(obrero_down, obrero.x, obrero.y, 0);
				}
				else
				{
					al_draw_bitmap(obrero_right, obrero.x, obrero.y, 0);
				}
			}
			else
			{
				if (tecla_up)
				{
					al_draw_bitmap(obrero_up, obrero.x, obrero.y, 0);
				}
				else if (tecla_down)
				{
					al_draw_bitmap(obrero_down, obrero.x, obrero.y, 0);
				}
				else
				{
					al_draw_bitmap(obrero_left, obrero.x, obrero.y, 0);
				}
			}
			//Dibujo obrero.

			//Textos.
			snprintf(posicion_texto, sizeof(posicion_texto), "Posición: (%d, %d)", obrero.x, obrero.y);
			al_draw_text(font_pos, al_map_rgb(0, 0, 0), 10, 10, 0, posicion_texto);
			snprintf(conpiedra_texto, sizeof(conpiedra_texto), "Piedra: %d", contpiedra);
			al_draw_text(font_piedra, al_map_rgb(0, 0, 0), 10, 30, 0, conpiedra_texto);
			snprintf(conmadera_texto, sizeof(conmadera_texto), "Madera: %d", contmadera);
			al_draw_text(font_madera, al_map_rgb(0, 0, 0), 10, 50, 0, conmadera_texto);
			//Textos.

			//Funciones de recolección.
			recolectar_madera(archivo);
			recolectar_piedra(archivo);
			//Funciones de recolección.

			//Actualización dibujos.
			al_flip_display();
			//Actualización dibujos.

			//Colisión con objetos.
			if (tecla_up && obrero.y > 0)
			{
				y_sig = (obrero.y - obrero.vel) / 40;
				x_act = obrero.x / 40;
				if (archivo[y_sig][x_act] != 'm' && archivo[y_sig][x_act] != 'a' && archivo[y_sig][x_act] != 'r' && archivo[y_sig][x_act] != 'n' && archivo[y_sig][x_act] != 'b' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'm' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'a' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'r' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'n' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'b')
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
				if (archivo[y_sig][x_act] != 'm' && archivo[y_sig][x_act] != 'a' && archivo[y_sig][x_act] != 'r' && archivo[y_sig][x_act] != 'n' && archivo[y_sig][x_act] != 'b' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'm' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'a' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'r' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'n' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'b')
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
				if (archivo[y_act][x_sig] != 'm' && archivo[y_act][x_sig] != 'a' && archivo[y_act][x_sig] != 'r' && archivo[y_act][x_sig] != 'n' && archivo[y_act][x_sig] != 'b' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'm' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'a' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'r' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'n' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'b')
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
				if (archivo[y_act][x_sig] != 'm' && archivo[y_act][x_sig] != 'a' && archivo[y_act][x_sig] != 'r' && archivo[y_act][x_sig] != 'n' && archivo[y_act][x_sig] != 'b' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'm' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'a' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'r' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'n' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'b')
				{
					obrero.x += obrero.vel;
				}
				else
				{
					obrero.x = x_sig * 40 - ANCHOPERSONAJE;
				}
			}
			//Colisión con objetos.

		}
		//Cuerpo juego.

		//Cerrar juego.
		else if (juego.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			ejecutandose = false;
		}
		//Cerrar juego.

		//Llamada función teclado_input.
		else if (juego.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			proceso_teclado_input(juego);
		}
		//Llamada función teclado_input.

		//LLamada función teclado_output.
		else if (juego.type == ALLEGRO_EVENT_KEY_UP)
		{
			proceso_teclado_output(juego);
		}
		//LLamada función teclado_output.
	}
	//Ejecución del juego.

	//Liberación de memoria.
	al_destroy_bitmap(obrero_right);
	al_destroy_bitmap(obrero_left);
	al_destroy_bitmap(obrero_up);
	al_destroy_bitmap(obrero_down);
	al_destroy_bitmap(muro);
	al_destroy_bitmap(camino);
	al_destroy_bitmap(cespedycamino);
	al_destroy_bitmap(cespedycamino2);
	al_destroy_bitmap(cesped);
	al_destroy_bitmap(arbol);
	al_destroy_bitmap(roca);
	al_destroy_bitmap(nucleo);
	al_destroy_bitmap(nucleo_borde);
	al_destroy_timer(fps);
	al_destroy_event_queue(eventos);
	al_destroy_display(ventana);
	al_destroy_font(font_pos);
	al_destroy_font(font_piedra);
	al_destroy_font(font_madera);
	//Liberación de memoria.

	return 0;
}
//Función principal.