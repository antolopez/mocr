using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;


namespace OCR
{
    public partial class ConfiguracionGeneralForm : Form
    {
        private Configuracion configuracionGeneral;

        public ConfiguracionGeneralForm(PrincipalForm padre)
        {
            InitializeComponent();

            configuracionGeneral = padre.configuracionGeneral;            

            ActualizarPerfiles();

            if (configuracionGeneral.perfilInicial != null && perfilInicialComboBox.Items.Contains(configuracionGeneral.perfilInicial))
                perfilInicialComboBox.SelectedItem = configuracionGeneral.perfilInicial;
            else
                perfilInicialComboBox.SelectedItem = "Predefinido";
            
        }

        private void aceptarButton_Click(object sender, EventArgs e)
        {
            configuracionGeneral.perfilInicial = perfilInicialComboBox.SelectedItem.ToString();

            if(!configuracionGeneral.Guardar())
                MessageBox.Show("Error al guardar la configuración. ", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

            Close();
        }

        private void ActualizarPerfiles()
        {
            String perfilSeleccionado = (String)perfilInicialComboBox.SelectedItem;

            perfilInicialComboBox.Items.Clear();

            //Cargamos los distintos perfiles disponibles en el combo box
            if (!Directory.Exists(Application.StartupPath + "\\Perfiles"))
                Directory.CreateDirectory(Application.StartupPath + "\\Perfiles");

            String[] perfiles = Directory.GetFiles(Application.StartupPath + "\\Perfiles");

            foreach (String perfil in perfiles)
                perfilInicialComboBox.Items.Add(perfil.Substring(perfil.LastIndexOf("\\") + 1));            
        }
    }
}