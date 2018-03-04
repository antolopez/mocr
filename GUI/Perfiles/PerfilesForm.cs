using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace OCR.Perfiles
{
    public partial class PerfilesForm : Form
    {
        private PrincipalForm padre;

        public PerfilesForm(PrincipalForm padre)
        {
            InitializeComponent();

            this.padre = padre;

            ActualizarPerfiles();
        }

        private void aceptarButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void nuevoPerfilButton_Click(object sender, EventArgs e)
        {
            NuevoPerfilForm nuevoPerfil = new NuevoPerfilForm(padre);

            nuevoPerfil.ShowDialog();

            ActualizarPerfiles();
        }

        private void editarPerfilButton_Click(object sender, EventArgs e)
        {
            if (perfilesActualesListBox.SelectedItem != null)
            {
                Perfil perfilEditado = new Perfil();

                if(!perfilEditado.Cargar(perfilesActualesListBox.SelectedItem.ToString()))
                {
                    MessageBox.Show("Error al cargar el perfil. ", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

                PerfilForm perfil = new PerfilForm(perfilEditado, "General");

                perfil.ShowDialog();
            }
        }

        private void eliminarPerfilButton_Click(object sender, EventArgs e)
        {
            if (perfilesActualesListBox.SelectedItem != null)
            {
                //Pedimos confirmación
                if (MessageBox.Show("¿Desea eliminar el perfil permanentemente?", "Confirmación", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                {
                    try
                    {
                        File.Delete(Application.StartupPath + "\\Perfiles\\" + perfilesActualesListBox.SelectedItem.ToString());

                        padre.textoActual.BorrarPerfil(Application.StartupPath + "\\BBDD\\bbdd.db", perfilesActualesListBox.SelectedItem.ToString());
                    }
                    catch (IOException)
                    {

                    }

                    ActualizarPerfiles();
                }
            }

        }

        private void ActualizarPerfiles()
        {
            perfilesActualesListBox.Items.Clear();

            //Cargamos los distintos perfiles disponibles en el list box
            if (!Directory.Exists(Application.StartupPath + "\\Perfiles"))
                Directory.CreateDirectory(Application.StartupPath + "\\Perfiles");

            String[] perfiles = Directory.GetFiles(Application.StartupPath + "\\Perfiles");

            foreach (String perfil in perfiles)
                perfilesActualesListBox.Items.Add(perfil.Substring(perfil.LastIndexOf("\\") + 1));
        }

        private void perfilesActualesListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (perfilesActualesListBox.SelectedItem != null)
            {
                editarPerfilButton.Enabled = true;
                eliminarPerfilButton.Enabled = true;
            }
        }

        private void perfilesActualesListBox_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                int posicionLista = perfilesActualesListBox.IndexFromPoint(e.X, e.Y);
                if (posicionLista == -1)
                {
                    perfilesContextMenuStrip.Items[1].Enabled = false;
                    perfilesContextMenuStrip.Items[2].Enabled = false;
                }
                else
                {
                    perfilesContextMenuStrip.Items[1].Enabled = true;
                    perfilesContextMenuStrip.Items[2].Enabled = true;
                    perfilesActualesListBox.SelectedIndex = posicionLista;
                }

            }
        }
    }
}