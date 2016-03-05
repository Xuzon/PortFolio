using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Windows.Forms;

namespace CaracolDesktop
{
    class Alga{
        public Image imagenActual;
        Image[] imagenes;
        int estado;
        int contador;
        public Point posicion;
        public Alga(){
            inicializar();
        }
        public Alga(int posX,int posY) {
            posicion = new Point(posX, posY);
            estado = 0;
            contador = 0;
            inicializarImagenes();
        }
        public void inicializar() {
            Random r = new Random();
            estado = 0;
            contador = 0;
            posicion = new Point(r.Next(0,Screen.PrimaryScreen.Bounds.Width),r.Next(0,Screen.PrimaryScreen.Bounds.Height)); //una nueva posicion
            inicializarImagenes();
        }
        void inicializarImagenes(){
            imagenes = new Image[4];//cuando tenga mas imagenes toca cambiar esto
            imagenes[0] = Properties.Resources.Alga1;
            imagenes[1] = Properties.Resources.Alga2;
            imagenes[2] = Properties.Resources.Alga3;
            imagenes[3] = Properties.Resources.Alga4;
            
            imagenActual = imagenes[0];
        }

        public void Update(){
           if( contador >= 50){ //si ya llamaron 10 veces a update
               contador = 0;
               if (estado < (imagenes.Length - 1)) {
                   estado++;
                   imagenActual = imagenes[estado];
                   return;
               }
               if (estado == (imagenes.Length - 1))
                   return;
           }
           contador++;
        }
    }
}
