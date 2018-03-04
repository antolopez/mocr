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
    public partial class NuevoPerfilForm : Form
    {
        private PrincipalForm padre;

        public NuevoPerfilForm(PrincipalForm padre)
        {
            InitializeComponent();

            this.padre= padre;

            //Cargamos los distintos perfiles disponibles en el list box
            if (!Directory.Exists(Application.StartupPath + "\\Perfiles"))
                Directory.CreateDirectory(Application.StartupPath + "\\Perfiles");

            String[] perfiles = Directory.GetFiles(Application.StartupPath + "\\Perfiles");

            foreach (String perfil in perfiles)
                perfilBaseComboBox.Items.Add(perfil.Substring(perfil.LastIndexOf("\\") + 1));

            perfilBaseComboBox.SelectedItem = "Predefinido";

        }

        private void aceptarButton_Click(object sender, EventArgs e)
        {
            try
            {
                File.Copy(Application.StartupPath + "\\Perfiles\\" + perfilBaseComboBox.SelectedItem, Application.StartupPath + "\\Perfiles\\" + nombrePerfilTextBox.Text);

                padre.textoActual.InsertarPerfil(Application.StartupPath + "\\BBDD\\bbdd.db", nombrePerfilTextBox.Text);

                Close();
            }
            catch (IOException)
            {
                MessageBox.Show("Ya existe un perfil con ese nombre.", "Advertencia", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }
    }
}