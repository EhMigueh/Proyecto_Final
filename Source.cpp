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
#define x_arreglo 41
#define y_arreglo 20
#define tam_recurso 150
//#define ESPACIORECURSO 150
//Valores definidos.

//Inicializaciones de ALLEGRO.
ALLEGRO_DISPLAY* ventana = NULL;
ALLEGRO_EVENT_QUEUE* eventos = NULL;
ALLEGRO_TIMER* temporizador = NULL;
ALLEGRO_BITMAP* obrero_derecha = NULL;
ALLEGRO_BITMAP* obrero_izquierda = NULL;
ALLEGRO_BITMAP* obrero_arriba = NULL;
ALLEGRO_BITMAP* obrero_abajo = NULL;
ALLEGRO_BITMAP* muro = NULL;
ALLEGRO_BITMAP* camino = NULL;
ALLEGRO_BITMAP* cespedycamino = NULL;
ALLEGRO_BITMAP* cespedycamino2 = NULL;
ALLEGRO_BITMAP* cesped = NULL;
ALLEGRO_BITMAP* arbol = NULL;
ALLEGRO_BITMAP* roca = NULL;
ALLEGRO_BITMAP* nucleo = NULL;
ALLEGRO_BITMAP* nucleo_borde = NULL;
ALLEGRO_BITMAP* torre_arqueros = NULL;
ALLEGRO_BITMAP* torre_magos = NULL;
ALLEGRO_BITMAP* torre_canon = NULL;
ALLEGRO_FONT* fuente_posicion = NULL;
ALLEGRO_FONT* fuente_piedra = NULL;
ALLEGRO_FONT* fuente_madera = NULL;
ALLEGRO_FILE* mapa = al_fopen("mapa.txt", "r");
//Inicializaciones de ALLEGRO.

//Variables globales.
int tecla_arriba = false;
int tecla_abajo = false;
int tecla_izquierda = false;
int tecla_derecha = false;
int contador_piedra = 0;
int contador_madera = 0;
bool ejecutandose = true;
bool recolectando = false;
time_t tiempo_ultima_recoleccion = 0;
//Variables globales.

//Estructuras.
struct _personaje
{
	int x;
	int y;
	float velocidad;
	bool direccion;
};
typedef struct _personaje personaje;
personaje obrero = { ANCHO / 2 - ANCHOPERSONAJE / 2, ALTO / 2 - ALTOPERSONAJE / 2, 2, false};
struct _enemigo
{
	int x;
	int y;
	int vida;
	float velocidad;
};
typedef struct _enemigo enemigo;
struct _recurso
{
	int x;
	int y;
	int cantidad;
};
typedef struct _recurso recurso;
/*
struct _torre
{
	int x;
	int y;
	int tipo;
	int daño;
	int rango;
};
typedef struct _torre torre;
*/
//Estructuras.

//Función para leer el mapa.
void lector_mapa(char archivo[y_arreglo][x_arreglo])
{
	int x_mapa, y_mapa;

	for (y_mapa = 0; y_mapa < y_arreglo; y_mapa++)
	{
		for (x_mapa = 0; x_mapa < x_arreglo; x_mapa++)
		{
			archivo[y_mapa][x_mapa] = al_fgetc(mapa);
		}
	}
	al_fclose(mapa);
}
//Función para leer el mapa.

//Función para inicializar recursos.
void inicializar_recursos(char archivo[y_arreglo][x_arreglo], recurso bosque[tam_recurso], recurso pedreria[tam_recurso])
{
	int y_mapa, x_mapa;
	int contador_arboles = 0;
	int contador_rocas = 0;

	for (y_mapa = 0; y_mapa < y_arreglo; y_mapa++)
	{
		for (x_mapa = 0; x_mapa < x_arreglo; x_mapa++)
		{
			if (archivo[y_mapa][x_mapa] == 'a')
			{
				bosque[contador_arboles].y = y_mapa;
				bosque[contador_arboles].x = x_mapa;
				bosque[contador_arboles].cantidad = 30;
				contador_arboles++;
			}
			if (archivo[y_mapa][x_mapa] == 'r')
			{
				pedreria[contador_rocas].y = y_mapa;
				pedreria[contador_rocas].x = x_mapa;
				pedreria[contador_rocas].cantidad = 20;
				contador_rocas++;
			}
		}
	}
}
//Función para inicializar recursos.

//Función encargada de la colision con los bordes.
void colision_bordes()
{
	if (tecla_arriba && obrero.y > 0)
	{
		obrero.y -= obrero.velocidad;
	}
	if (tecla_abajo && obrero.y + ALTOPERSONAJE < ALTO)
	{
		obrero.y += obrero.velocidad;
	}
	if (tecla_izquierda && obrero.x > 0)
	{
		obrero.x -= obrero.velocidad;
		obrero.direccion = false;
	}
	if (tecla_derecha && obrero.x + ANCHOPERSONAJE < ANCHO)
	{
		obrero.x += obrero.velocidad;
		obrero.direccion = true;
	}
}
//Función encargada de la colision con los bordes.

//Colisión con objetos.
void colision_objetos(char archivo[y_arreglo][x_arreglo])
{
	int x_act, y_act;
	int x_sig, y_sig;

	if (tecla_arriba && obrero.y > 0)
	{
		y_sig = (obrero.y - obrero.velocidad) / 40;
		x_act = obrero.x / 40;
		if (archivo[y_sig][x_act] != 'm' && archivo[y_sig][x_act] != 'a' && archivo[y_sig][x_act] != 'r' && archivo[y_sig][x_act] != 'n' && archivo[y_sig][x_act] != 'b' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'm' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'a' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'r' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'n' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'b')
		{
			obrero.y -= obrero.velocidad;
		}
		else
		{
			obrero.y = (y_sig + 1) * 40;
		}
	}
	if (tecla_abajo && obrero.y + ALTOPERSONAJE < ALTO)
	{
		y_sig = (obrero.y + obrero.velocidad + ALTOPERSONAJE) / 40;
		x_act = obrero.x / 40;
		if (archivo[y_sig][x_act] != 'm' && archivo[y_sig][x_act] != 'a' && archivo[y_sig][x_act] != 'r' && archivo[y_sig][x_act] != 'n' && archivo[y_sig][x_act] != 'b' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'm' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'a' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'r' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'n' && archivo[y_sig][(obrero.x + ANCHOPERSONAJE - 1) / 40] != 'b')
		{
			obrero.y += obrero.velocidad;
		}
		else
		{
			obrero.y = y_sig * 40 - ALTOPERSONAJE;
		}
	}
	if (tecla_izquierda && obrero.x > 0)
	{
		y_act = obrero.y / 40;
		x_sig = (obrero.x - obrero.velocidad) / 40;
		if (archivo[y_act][x_sig] != 'm' && archivo[y_act][x_sig] != 'a' && archivo[y_act][x_sig] != 'r' && archivo[y_act][x_sig] != 'n' && archivo[y_act][x_sig] != 'b' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'm' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'a' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'r' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'n' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'b')
		{
			obrero.x -= obrero.velocidad;
		}
		else
		{
			obrero.x = (x_sig + 1) * 40;
		}
	}
	if (tecla_derecha && obrero.x + ANCHOPERSONAJE < ANCHO)
	{
		y_act = obrero.y / 40;
		x_sig = (obrero.x + obrero.velocidad + ANCHOPERSONAJE) / 40;
		if (archivo[y_act][x_sig] != 'm' && archivo[y_act][x_sig] != 'a' && archivo[y_act][x_sig] != 'r' && archivo[y_act][x_sig] != 'n' && archivo[y_act][x_sig] != 'b' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'm' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'a' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'r' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'n' && archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_sig] != 'b')
		{
			obrero.x += obrero.velocidad;
		}
		else
		{
			obrero.x = x_sig * 40 - ANCHOPERSONAJE;
		}
	}
}
//Colisión con objetos.

//Función para dibujar objetos.
void dibujar_objetos(char archivo[y_arreglo][x_arreglo], ALLEGRO_BITMAP* muro, ALLEGRO_BITMAP* camino, ALLEGRO_BITMAP* cespedycamino, ALLEGRO_BITMAP* cespedycamino2, ALLEGRO_BITMAP* cesped, ALLEGRO_BITMAP* arbol, ALLEGRO_BITMAP* roca, ALLEGRO_BITMAP* nucleo, ALLEGRO_BITMAP* nucleo_borde, ALLEGRO_BITMAP* torre_arqueros, ALLEGRO_BITMAP* torre_magos, ALLEGRO_BITMAP* torre_canon)
{
	int x_mapa, y_mapa;

	for (y_mapa = 0; y_mapa < y_arreglo; y_mapa++)
	{
		for (x_mapa = 0; x_mapa < x_arreglo; x_mapa++)
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
			else if (archivo[y_mapa][x_mapa] == 'u')
			{
				al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'h')
			{
				al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
				al_draw_bitmap(torre_arqueros, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'j')
			{
				al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
				al_draw_bitmap(torre_magos, x_mapa * 40, y_mapa * 40, 0);
			}
			else if (archivo[y_mapa][x_mapa] == 'g')
			{
				al_draw_bitmap(cesped, x_mapa * 40, y_mapa * 40, 0);
				al_draw_bitmap(torre_canon, x_mapa * 40, y_mapa * 40, 0);
			}
		}
	}
}
//Función para dibujar objetos.

//Función para dibujar obrero.
void dibujar_obrero()
{
	if (obrero.direccion)
	{
		if (tecla_arriba)
		{
			al_draw_bitmap(obrero_arriba, obrero.x, obrero.y, 0);
		}
		else if (tecla_abajo)
		{
			al_draw_bitmap(obrero_abajo, obrero.x, obrero.y, 0);
		}
		else
		{
			al_draw_bitmap(obrero_derecha, obrero.x, obrero.y, 0);
		}
	}
	else
	{
		if (tecla_arriba)
		{
			al_draw_bitmap(obrero_arriba, obrero.x, obrero.y, 0);
		}
		else if (tecla_abajo)
		{
			al_draw_bitmap(obrero_abajo, obrero.x, obrero.y, 0);
		}
		else
		{
			al_draw_bitmap(obrero_izquierda, obrero.x, obrero.y, 0);
		}
	}
}
//Función para dibujar obrero.

//Función para dibujar textos.
void dibujar_textos(ALLEGRO_FONT* fuente_posicion, ALLEGRO_FONT* fuente_piedra, ALLEGRO_FONT* fuente_madera)
{
	char posicion_texto[50];
	char conpiedra_texto[50];
	char conmadera_texto[50];

	snprintf(posicion_texto, sizeof(posicion_texto), "Posicion: (%d, %d)", obrero.x, obrero.y);
	al_draw_text(fuente_posicion, al_map_rgb(0, 0, 0), 10, 10, 0, posicion_texto);
	snprintf(conpiedra_texto, sizeof(conpiedra_texto), "Piedra: %d", contador_piedra);
	al_draw_text(fuente_piedra, al_map_rgb(0, 0, 0), 10, 30, 0, conpiedra_texto);
	snprintf(conmadera_texto, sizeof(conmadera_texto), "Madera: %d", contador_madera);
	al_draw_text(fuente_madera, al_map_rgb(0, 0, 0), 10, 50, 0, conmadera_texto);
}
//Función para dibujar textos.

//Función para construir torre de arqueros.
void construir_torre_arqueros(char archivo[y_arreglo][x_arreglo])
{
	int x_mapa, y_mapa;

	x_mapa = obrero.x / 40;
	y_mapa = obrero.y / 40;

	if (archivo[y_mapa][x_mapa] == 'u' && contador_madera >= 30 && contador_piedra >= 5)
	{
		archivo[y_mapa][x_mapa] = 'h';
		contador_madera -= 30;
		contador_piedra -= 5;
	}
}
//Función para construir torre de arqueros.

//Función para construir torre de magos.
void construir_torre_magos(char archivo[y_arreglo][x_arreglo])
{
	int x_mapa, y_mapa;

	x_mapa = obrero.x / 40;
	y_mapa = obrero.y / 40;

	if (archivo[y_mapa][x_mapa] == 'u' && contador_madera >= 5 && contador_piedra >= 30)
	{
		archivo[y_mapa][x_mapa] = 'j';
		contador_madera -= 5;
		contador_piedra -= 30;
	}
}
//Función para construir torre de magos.

//Función para construir torre de cañon.
void construir_torre_canon(char archivo[y_arreglo][x_arreglo])
{
	int x_mapa, y_mapa;

	x_mapa = obrero.x / 40;
	y_mapa = obrero.y / 40;

	if (archivo[y_mapa][x_mapa] == 'u' && contador_madera >= 25 && contador_piedra >= 25)
	{
		archivo[y_mapa][x_mapa] = 'g';
		contador_madera -= 25;
		contador_piedra -= 25;
	}
}
//Función para construir torre de cañon.

//Función para buscar recursos.
int buscar_recurso(int x_mapa, int y_mapa, recurso tipo_recurso[tam_recurso])
{
	int posicion = 0;

	for (posicion = 0; posicion <= tam_recurso; posicion++)
	{
		if (tipo_recurso[posicion].x == x_mapa && tipo_recurso[posicion].y == y_mapa)
		{
			return posicion;
		}
	}
	return -1;
}
//Función para buscar recursos.

//Recolector y contador piedra.
void recolectar_piedra(char archivo[y_arreglo][x_arreglo], recurso pedreria[tam_recurso])
{
	int x_mapa, y_mapa;
	int indice;
	time_t tiempo_actual;

	x_mapa = obrero.x / 40;
	y_mapa = obrero.y / 40;

	if (recolectando)
	{
		if (archivo[y_mapa][x_mapa] == 'r')
		{
			indice = buscar_recurso(x_mapa, y_mapa, pedreria);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ultima_recoleccion >= 2)
			{
				contador_piedra = contador_piedra + 5;
				pedreria[indice].cantidad -= 5;
				if (pedreria[indice].cantidad <= 0)
				{
					archivo[y_mapa][x_mapa] = 'o';
				}
				tiempo_ultima_recoleccion = tiempo_actual;
			}
		}
		else if (archivo[y_mapa][(obrero.x + ANCHOPERSONAJE - 1) / 40] == 'r')
		{
			indice = buscar_recurso(x_mapa, y_mapa, pedreria);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ultima_recoleccion >= 2)
			{
				contador_piedra = contador_piedra + 5;
				pedreria[indice].cantidad -= 5;
				if (pedreria[indice].cantidad <= 0)
				{
					archivo[y_mapa][(obrero.x + ANCHOPERSONAJE - 1) / 40] = 'o';
				}
				tiempo_ultima_recoleccion = tiempo_actual;
			}
		}
		else if (archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_mapa] == 'r')
		{
			indice = buscar_recurso(x_mapa, y_mapa, pedreria);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ultima_recoleccion >= 2)
			{
				contador_piedra = contador_piedra + 5;
				pedreria[indice].cantidad -= 5;
				if (pedreria[indice].cantidad <= 0)
				{
					archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_mapa] = 'o';
				}
				tiempo_ultima_recoleccion = tiempo_actual;
			}
		}
		else if (archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][(obrero.x + ANCHOPERSONAJE - 1) / 40] == 'r')
		{
			indice = buscar_recurso(x_mapa, y_mapa, pedreria);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ultima_recoleccion >= 2)
			{
				contador_piedra = contador_piedra + 5;
				pedreria[indice].cantidad -= 5;
				if (pedreria[indice].cantidad <= 0)
				{
					archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][(obrero.x + ANCHOPERSONAJE - 1) / 40] = 'o';
				}
				tiempo_ultima_recoleccion = tiempo_actual;
			}
		}
	}
}
//Recolector y contador piedra.

//Recolector y contador madera.
void recolectar_madera(char archivo[y_arreglo][x_arreglo], recurso bosque[tam_recurso])
{
	int x_mapa, y_mapa;
	int indice;
	time_t tiempo_actual;

	x_mapa = obrero.x / 40;
	y_mapa = obrero.y / 40;

	if (recolectando)
	{
		if (archivo[y_mapa][x_mapa] == 'a')
		{
			indice = buscar_recurso(x_mapa, y_mapa, bosque);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ultima_recoleccion >= 2)
			{
				contador_madera = contador_madera + 5;
				bosque[indice].cantidad -= 5;
				if (bosque[indice].cantidad <= 0)
				{
					archivo[y_mapa][x_mapa] = 'o';
				}
				tiempo_ultima_recoleccion = tiempo_actual;
			}
		}
		else if (archivo[y_mapa][(obrero.x + ANCHOPERSONAJE - 1) / 40] == 'a')
		{
			indice = buscar_recurso(x_mapa, y_mapa, bosque);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ultima_recoleccion >= 2)
			{
				contador_madera = contador_madera + 5;
				bosque[indice].cantidad -= 5;
				if (bosque[indice].cantidad <= 0)
				{
					archivo[y_mapa][(obrero.x + ANCHOPERSONAJE - 1) / 40] = 'o';
				}
				tiempo_ultima_recoleccion = tiempo_actual;
			}
		}
		else if (archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_mapa] == 'a')
		{
			indice = buscar_recurso(x_mapa, y_mapa, bosque);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ultima_recoleccion >= 2)
			{
				contador_madera = contador_madera + 5;
				bosque[indice].cantidad -= 5;
				if (bosque[indice].cantidad <= 0)
				{
					archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][x_mapa] = 'o';
				}
				tiempo_ultima_recoleccion = tiempo_actual;
			}
		}
		else if (archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][(obrero.x + ANCHOPERSONAJE - 1) / 40] == 'a')
		{
			indice = buscar_recurso(x_mapa, y_mapa, bosque);
			tiempo_actual = time(NULL);
			if (tiempo_actual - tiempo_ultima_recoleccion >= 2)
			{
				contador_madera = contador_madera + 5;
				bosque[indice].cantidad -= 5;
				if (bosque[indice].cantidad <= 0)
				{
					archivo[(obrero.y + ALTOPERSONAJE - 1) / 40][(obrero.x + ANCHOPERSONAJE - 1) / 40] = 'o';
				}
				tiempo_ultima_recoleccion = tiempo_actual;
			}
		}
	}
}
//Recolector y contador madera.

//Función encargada del teclado cuando se presiona una tecla.
void proceso_teclado_input(ALLEGRO_EVENT juego, char archivo[y_arreglo][x_arreglo])
{
	switch (juego.keyboard.keycode)
	{
	case ALLEGRO_KEY_UP: tecla_arriba = true;
		break;
	case ALLEGRO_KEY_DOWN: tecla_abajo = true;
		break;
	case ALLEGRO_KEY_LEFT: tecla_izquierda = true;
		break;
	case ALLEGRO_KEY_RIGHT: tecla_derecha = true;
		break;
	case ALLEGRO_KEY_Q: recolectando = true;
		break;
	case ALLEGRO_KEY_W: construir_torre_arqueros(archivo);
		break;
	case ALLEGRO_KEY_E: construir_torre_magos(archivo);
		break;
	case ALLEGRO_KEY_R: construir_torre_canon(archivo);
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
	case ALLEGRO_KEY_UP: tecla_arriba = false;
		break;
	case ALLEGRO_KEY_DOWN: tecla_abajo = false;
		break;
	case ALLEGRO_KEY_LEFT: tecla_izquierda = false;
		break;
	case ALLEGRO_KEY_RIGHT: tecla_derecha = false;
		break;
	case ALLEGRO_KEY_Q: recolectando = false;
		break;
	}
}
//Función encargada de teclado cuando se deja de pulsar una tecla.

//Función principal.
int main()
{
	//Declaración variables locacles función principal.
	char archivo[y_arreglo][x_arreglo];

	tiempo_ultima_recoleccion = time(NULL);

	recurso bosque[tam_recurso];
	recurso pedreria[tam_recurso];
	recurso tipo_recurso[tam_recurso];
	//Declaración variables locacles función principal.

	//Errores e inicializaciones de ALLEGRO.
	if (!al_init())
	{
		printf("Error al iniciar ALLEGRO");
		return -1;
	}
	ventana = al_create_display(ANCHO, ALTO);
	al_install_keyboard();
	eventos = al_create_event_queue();
	if (!ventana || !al_install_keyboard() || !eventos)
	{
		printf("Error al abrir la ventana o instalar teclado o crear cola de eventos");
		al_destroy_display(ventana);
		return -1;
	}
	temporizador = al_create_timer(1.0 / 60);
	if (!temporizador)
	{
		printf("Error al crear temporizador");
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	al_init_image_addon();
	obrero_derecha = al_load_bitmap("obrero_right.png");
	obrero_izquierda = al_load_bitmap("obrero_left.png");
	obrero_arriba = al_load_bitmap("obrero_up.png");
	obrero_abajo = al_load_bitmap("obrero_down.png");
	muro = al_load_bitmap("muro.png");
	camino = al_load_bitmap("camino.png");
	cespedycamino = al_load_bitmap("cespedycamino.png");
	cespedycamino2 = al_load_bitmap("cespedycamino2.png");
	cesped = al_load_bitmap("cesped.png");
	arbol = al_load_bitmap("arbol.png");
	roca = al_load_bitmap("roca.png");
	nucleo = al_load_bitmap("nucleo.png");
	nucleo_borde = al_load_bitmap("nucleo_borde.png");
	torre_arqueros = al_load_bitmap("torre_arqueros.png");
	torre_magos = al_load_bitmap("torre_magos.png");
	torre_canon = al_load_bitmap("torre_canon.png");
	if (!obrero_derecha || !obrero_izquierda || !obrero_arriba || !obrero_abajo || !muro || !camino || !cespedycamino || !cespedycamino2 || !cesped || !arbol || !roca || !nucleo || !nucleo_borde || !torre_arqueros || !torre_magos || !torre_canon)
	{
		printf("Error al cargar un dibujo de objeto dentro del mapa");
		al_destroy_timer(temporizador);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	al_init_font_addon();
	al_init_ttf_addon();
	fuente_posicion = al_load_font("arial.ttf", 20, 0);
	fuente_piedra = al_load_font("arial.ttf", 20, 0);
	fuente_madera = al_load_font("arial.ttf", 20, 0);
	if (!fuente_posicion || !fuente_piedra || !fuente_madera)
	{
		printf("Error al cargar la fuente de la posición o fuente de la piedra o fuente de la madera");
		al_destroy_timer(temporizador);
		al_destroy_event_queue(eventos);
		al_destroy_display(ventana);
		return -1;
	}
	//Errores e inicializaciones de ALLEGRO.

	//Registro de eventos.
	al_register_event_source(eventos, al_get_display_event_source(ventana));
	al_register_event_source(eventos, al_get_timer_event_source(temporizador));
	al_register_event_source(eventos, al_get_keyboard_event_source());
	al_set_window_title(ventana, "Protect the CORE");
	al_flip_display();
	al_start_timer(temporizador);
	//Registro de eventos.

	//Lector de mapa.
	lector_mapa(archivo);
	//Lector de mapa.

	//Inicialización de recursos.
	inicializar_recursos(archivo, bosque, pedreria);
	//Inicialización de recursos.

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
			colision_bordes();
			//Llamada función de colisión con bordes.

			//Limpieza de los dibujos.
			al_clear_to_color(al_map_rgb(0, 0, 0));
			//Limpieza de los dibujos.

			//Dibujo objetos.
			dibujar_objetos(archivo, muro, camino, cespedycamino, cespedycamino2, cesped, arbol, roca, nucleo, nucleo_borde, torre_arqueros, torre_magos, torre_canon);
			//Dibujo objetos.

			//Dibujo obrero.
			dibujar_obrero();
			//Dibujo obrero.

			//Dibujo textos.
			dibujar_textos(fuente_posicion, fuente_piedra, fuente_madera);
			//Dibujo textos.

			//Funciones de recolección.
			recolectar_madera(archivo, bosque);
			recolectar_piedra(archivo, pedreria);
			//Funciones de recolección.

			//Actualización dibujos.
			al_flip_display();
			//Actualización dibujos.

			//colisión con objetos.
			colision_objetos(archivo);
			//colisión con objetos.
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
			proceso_teclado_input(juego, archivo);
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
	al_destroy_bitmap(obrero_derecha);
	al_destroy_bitmap(obrero_izquierda);
	al_destroy_bitmap(obrero_arriba);
	al_destroy_bitmap(obrero_abajo);
	al_destroy_bitmap(muro);
	al_destroy_bitmap(camino);
	al_destroy_bitmap(cespedycamino);
	al_destroy_bitmap(cespedycamino2);
	al_destroy_bitmap(cesped);
	al_destroy_bitmap(arbol);
	al_destroy_bitmap(roca);
	al_destroy_bitmap(nucleo);
	al_destroy_bitmap(nucleo_borde);
	al_destroy_bitmap(torre_arqueros);
	al_destroy_bitmap(torre_magos);
	al_destroy_bitmap(torre_canon);
	al_destroy_timer(temporizador);
	al_destroy_event_queue(eventos);
	al_destroy_display(ventana);
	al_destroy_font(fuente_posicion);
	al_destroy_font(fuente_piedra);
	al_destroy_font(fuente_madera);
	//Liberación de memoria.

	return 0;
}
//Función principal.