using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;

namespace CaracolDesktop{
    public partial class Form1 : Form{
        Caracol caracol;
        Thread logic;
        Alga[] Algas;
        Rectangle pantalla;
        Random random;
        public Form1(){
            InitializeComponent();
            SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None; //le quito la toolbar y los bordes
            FormClosing += Form1_FormClosing; //añado que cuando Reciva el evento FormClosing vaya a la funcion
            Click += Form1_Click;
            //this.BackColor = Color.Magenta; //hago que el background sea magenta
            this.TransparencyKey = this.BackColor; //digo que el magenta es transparente
            pantalla = Screen.PrimaryScreen.Bounds; //cojo la resolucion de la pantalla
            this.WindowState = FormWindowState.Maximized;//maximizo la ventana
            this.MinimumSize = new Size(pantalla.Width,pantalla.Height);//hago que siempre esté a pantala completa
            this.MaximumSize = new Size(pantalla.Width,pantalla.Height);
            random = new Random(); //creo un nuevo random
            Algas = new Alga[10]; //inicializo el array de algas
            for (int i = 0; i < 10; i++)
                Algas[i] = new Alga(random.Next(0, Screen.PrimaryScreen.Bounds.Width), random.Next(0, Screen.PrimaryScreen.Bounds.Height));

            caracol = new Caracol(); //creo el caracol
            caracol.objetivo = Algas[random.Next(0, 10)]; //le pongo un objetivo al caracol
            logic = new Thread(Logica);//creo el hilo logico
            logic.Start();//lo inicio
        }

        void Form1_Click(object sender, EventArgs e){
            Point p = Cursor.Position;
            if (p.X >= caracol.posicion.X && p.X <= (caracol.posicion.X + caracol.Bounds.X) && p.Y >= caracol.posicion.Y && p.Y <= (caracol.posicion.Y + caracol.Bounds.Y)){
                caracol.cayendo = true;
                caracol.aceleracion = 1;
            }
        }

        void Form1_FormClosing(object sender, FormClosingEventArgs e){ //para cuando cierras la aplicacion por la X
            logic.Abort();
        }
        protected override void OnPaint(PaintEventArgs e){//pinto el caracol y las algas
                Point p = new Point(caracol.posicion.X, caracol.posicion.Y);
                //e.Graphics.Clear(Color.Magenta);
                e.Graphics.Flush();
                e.Graphics.DrawImage(caracol.imagen, p);
                for (int i = 0; i < 10; i++){
                    e.Graphics.DrawImage(Algas[i].imagenActual, Algas[i].posicion);
                }
                e.Graphics.Flush();
        }

        private void Logica(){
            while (true) {
                Debug.WriteLine("r: "+Color.Magenta.R+" g: "+Color.Magenta.G+" b: "+Color.Magenta.B);
                this.Invalidate();
                if (caracol.llegada){ //si el caracol llegó
                    for (int i = 0; i < Algas.Length; i++){ //compruebo que alga llegó
                        if (caracol.objetivo.Equals(Algas[i])) //la reinicializo
                            Algas[i].inicializar();
                    }
                    int r = new Random().Next(0, 10); //escojo un siguiente objetivo aleatorio
                    caracol.objetivo = Algas[r];
                    caracol.llegada = false;
                }
                caracol.Update();//qie el caracol se mueva
                foreach (Alga a in Algas)//hago que las algas crezcan
                    a.Update();
                Thread.Sleep(50);
            }
        }
    }
}
