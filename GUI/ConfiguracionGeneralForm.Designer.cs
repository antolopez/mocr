namespace OCR
{
    partial class ConfiguracionGeneralForm
    {
        /// <summary>
        /// Variable del diseñador requerida.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén utilizando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben eliminar; false en caso contrario, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido del método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.perfilesConfigGroupBox = new System.Windows.Forms.GroupBox();
            this.perfilInicialLabel = new System.Windows.Forms.Label();
            this.perfilInicialComboBox = new System.Windows.Forms.ComboBox();
            this.aceptarButton = new System.Windows.Forms.Button();
            this.cancelarButton = new System.Windows.Forms.Button();
            this.perfilesConfigGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // perfilesConfigGroupBox
            // 
            this.perfilesConfigGroupBox.Controls.Add(this.perfilInicialLabel);
            this.perfilesConfigGroupBox.Controls.Add(this.perfilInicialComboBox);
            this.perfilesConfigGroupBox.Location = new System.Drawing.Point(12, 12);
            this.perfilesConfigGroupBox.Name = "perfilesConfigGroupBox";
            this.perfilesConfigGroupBox.Size = new System.Drawing.Size(219, 64);
            this.perfilesConfigGroupBox.TabIndex = 0;
            this.perfilesConfigGroupBox.TabStop = false;
            this.perfilesConfigGroupBox.Text = "Perfiles";
            // 
            // perfilInicialLabel
            // 
            this.perfilInicialLabel.AutoSize = true;
            this.perfilInicialLabel.Location = new System.Drawing.Point(7, 27);
            this.perfilInicialLabel.Name = "perfilInicialLabel";
            this.perfilInicialLabel.Size = new System.Drawing.Size(62, 13);
            this.perfilInicialLabel.TabIndex = 3;
            this.perfilInicialLabel.Text = "Perfil inicial:";
            // 
            // perfilInicialComboBox
            // 
            this.perfilInicialComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.perfilInicialComboBox.FormattingEnabled = true;
            this.perfilInicialComboBox.Location = new System.Drawing.Point(75, 23);
            this.perfilInicialComboBox.Name = "perfilInicialComboBox";
            this.perfilInicialComboBox.Size = new System.Drawing.Size(121, 21);
            this.perfilInicialComboBox.TabIndex = 2;
            // 
            // aceptarButton
            // 
            this.aceptarButton.Location = new System.Drawing.Point(32, 91);
            this.aceptarButton.Name = "aceptarButton";
            this.aceptarButton.Size = new System.Drawing.Size(75, 23);
            this.aceptarButton.TabIndex = 1;
            this.aceptarButton.Text = "Aceptar";
            this.aceptarButton.UseVisualStyleBackColor = true;
            this.aceptarButton.Click += new System.EventHandler(this.aceptarButton_Click);
            // 
            // cancelarButton
            // 
            this.cancelarButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelarButton.Location = new System.Drawing.Point(142, 91);
            this.cancelarButton.Name = "cancelarButton";
            this.cancelarButton.Size = new System.Drawing.Size(75, 23);
            this.cancelarButton.TabIndex = 2;
            this.cancelarButton.Text = "Cancelar";
            this.cancelarButton.UseVisualStyleBackColor = true;
            // 
            // ConfiguracionGeneralForm
            // 
            this.AcceptButton = this.aceptarButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelarButton;
            this.ClientSize = new System.Drawing.Size(243, 121);
            this.Controls.Add(this.cancelarButton);
            this.Controls.Add(this.aceptarButton);
            this.Controls.Add(this.perfilesConfigGroupBox);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ConfiguracionGeneralForm";
            this.ShowIcon = false;
            this.Text = "Configuración General";
            this.perfilesConfigGroupBox.ResumeLayout(false);
            this.perfilesConfigGroupBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox perfilesConfigGroupBox;
        private System.Windows.Forms.Button aceptarButton;
        private System.Windows.Forms.Button cancelarButton;
        private System.Windows.Forms.Label perfilInicialLabel;
        private System.Windows.Forms.ComboBox perfilInicialComboBox;
    }
}