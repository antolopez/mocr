using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Imagen;

namespace OCR
{
    public partial class OpcionesKNNForm : Form
    {
        private TextoManejado texto;

        public OpcionesKNNForm(TextoManejado texto)
        {
            InitializeComponent();

            this.texto = texto;
            numeroVecinosNumericUpDown.Value = texto.GetNumeroVecinos();
        }

        private void aceptarButton_Click(object sender, EventArgs e)
        {
            texto.SetNumeroVecinos((int)numeroVecinosNumericUpDown.Value);

            Close();
        }
    }
}