namespace OCR.Perfiles
{
    partial class NuevoPerfilForm
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
            this.nombrePerfilLabel = new System.Windows.Forms.Label();
            this.nombrePerfilTextBox = new System.Windows.Forms.TextBox();
            this.perfilBaseLabel = new System.Windows.Forms.Label();
            this.aceptarButton = new System.Windows.Forms.Button();
            this.cancelarButton = new System.Windows.Forms.Button();
            this.perfilBaseComboBox = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // nombrePerfilLabel
            // 
            this.nombrePerfilLabel.AutoSize = true;
            this.nombrePerfilLabel.Location = new System.Drawing.Point(22, 24);
            this.nombrePerfilLabel.Name = "nombrePerfilLabel";
            this.nombrePerfilLabel.Size = new System.Drawing.Size(47, 13);
            this.nombrePerfilLabel.TabIndex = 0;
            this.nombrePerfilLabel.Text = "Nombre:";
            // 
            // nombrePerfilTextBox
            // 
            this.nombrePerfilTextBox.Location = new System.Drawing.Point(75, 21);
            this.nombrePerfilTextBox.Name = "nombrePerfilTextBox";
            this.nombrePerfilTextBox.Size = new System.Drawing.Size(193, 20);
            this.nombrePerfilTextBox.TabIndex = 1;
            // 
            // perfilBaseLabel
            // 
            this.perfilBaseLabel.AutoSize = true;
            this.perfilBaseLabel.Location = new System.Drawing.Point(9, 77);
            this.perfilBaseLabel.Name = "perfilBaseLabel";
            this.perfilBaseLabel.Size = new System.Drawing.Size(60, 13);
            this.perfilBaseLabel.TabIndex = 2;
            this.perfilBaseLabel.Text = "Perfil Base:";
            // 
            // aceptarButton
            // 
            this.aceptarButton.Location = new System.Drawing.Point(25, 124);
            this.aceptarButton.Name = "aceptarButton";
            this.aceptarButton.Size = new System.Drawing.Size(75, 23);
            this.aceptarButton.TabIndex = 3;
            this.aceptarButton.Text = "Aceptar";
            this.aceptarButton.UseVisualStyleBackColor = true;
            this.aceptarButton.Click += new System.EventHandler(this.aceptarButton_Click);
            // 
            // cancelarButton
            // 
            this.cancelarButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelarButton.Location = new System.Drawing.Point(179, 124);
            this.cancelarButton.Name = "cancelarButton";
            this.cancelarButton.Size = new System.Drawing.Size(75, 23);
            this.cancelarButton.TabIndex = 4;
            this.cancelarButton.Text = "Cancelar";
            this.cancelarButton.UseVisualStyleBackColor = true;
            // 
            // perfilBaseComboBox
            // 
            this.perfilBaseComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.perfilBaseComboBox.FormattingEnabled = true;
            this.perfilBaseComboBox.Location = new System.Drawing.Point(75, 74);
            this.perfilBaseComboBox.Name = "perfilBaseComboBox";
            this.perfilBaseComboBox.Size = new System.Drawing.Size(193, 21);
            this.perfilBaseComboBox.TabIndex = 5;
            // 
            // NuevoPerfilForm
            // 
            this.AcceptButton = this.aceptarButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelarButton;
            this.ClientSize = new System.Drawing.Size(280, 159);
            this.Controls.Add(this.perfilBaseComboBox);
            this.Controls.Add(this.cancelarButton);
            this.Controls.Add(this.aceptarButton);
            this.Controls.Add(this.perfilBaseLabel);
            this.Controls.Add(this.nombrePerfilTextBox);
            this.Controls.Add(this.nombrePerfilLabel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "NuevoPerfilForm";
            this.Text = "Nuevo Perfil";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label nombrePerfilLabel;
        private System.Windows.Forms.TextBox nombrePerfilTextBox;
        private System.Windows.Forms.Label perfilBaseLabel;
        private System.Windows.Forms.Button aceptarButton;
        private System.Windows.Forms.Button cancelarButton;
        private System.Windows.Forms.ComboBox perfilBaseComboBox;
    }
}