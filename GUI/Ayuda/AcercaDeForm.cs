using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace OCR.Ayuda
{
    public partial class AcercaDeForm : Form
    {
        public AcercaDeForm()
        {
            InitializeComponent();
        }

        private void aceptarButton_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}