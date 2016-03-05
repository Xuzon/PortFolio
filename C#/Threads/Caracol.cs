using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;

namespace CaracolDesktop
{
    class Caracol{
        public Point posicion;
        public Point Bounds;
        public Alga objetivo;
        public bool llegada;
        public Image imagen;
        public bool cayendo;
        public int aceleracion;
        Image[] imagenesAndar;
        int estado;

        public Caracol() {
            posicion = new Point(0,0);
            estado = 0;
            inicializarImagenes();
            Bounds = new Point(100, 100);
        }
        void inicializarImagenes() {
            imagenesAndar = new Image[2];
            imagenesAndar[0] = Properties.Resources.Snail;
            imagenesAndar[1] = Properties.Resources.Snail2;
            imagen = imagenesAndar[0];
        }
        public void Update(){
            if (cayendo){
                posicion.Y += aceleracion;
                aceleracion++;
                if (posicion.Y >= Screen.PrimaryScreen.Bounds.Height - Bounds.Y) {
                    cayendo = false;
                }
            }
            else
            {
                if (estado < (imagenesAndar.Length - 1)){ //en cada update sigo la animacion
                    estado++;
                }
                else estado = 0;

                imagen = imagenesAndar[estado];//despues de saber que imagen va a salir la asigno
                if (objetivo.posicion.X != posicion.X){ //andar a derecha o izquierda
                    if (objetivo.posicion.X > posicion.X)
                        posicion.X++;
                    else posicion.X--;
                }
                if (objetivo.posicion.Y != posicion.Y){//andar arriba o abajo
                    if (objetivo.posicion.Y > posicion.Y)
                        posicion.Y++;
                    else posicion.Y--;
                }
                if (objetivo.posicion.Equals(this.posicion)) //saber si llegué
                    llegada = true;
            }
        }
    }
}
