namespace OCR.Perfiles
{
    partial class PerfilesForm
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
            this.components = new System.ComponentModel.Container();
            this.perfilesActualesListBox = new System.Windows.Forms.ListBox();
            this.perfilesContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.nuevoPerfilToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editarPerfilToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.eliminarPerfilToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.perfilesActualesLabel = new System.Windows.Forms.Label();
            this.aceptarButton = new System.Windows.Forms.Button();
            this.eliminarPerfilButton = new System.Windows.Forms.Button();
            this.editarPerfilButton = new System.Windows.Forms.Button();
            this.nuevoPerfilButton = new System.Windows.Forms.Button();
            this.perfilesContextMenuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // perfilesActualesListBox
            // 
            this.perfilesActualesListBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.perfilesActualesListBox.ContextMenuStrip = this.perfilesContextMenuStrip;
            this.perfilesActualesListBox.FormattingEnabled = true;
            this.perfilesActualesListBox.Location = new System.Drawing.Point(31, 38);
            this.perfilesActualesListBox.Name = "perfilesActualesListBox";
            this.perfilesActualesListBox.Size = new System.Drawing.Size(254, 225);
            this.perfilesActualesListBox.TabIndex = 0;
            this.perfilesActualesListBox.SelectedIndexChanged += new System.EventHandler(this.perfilesActualesListBox_SelectedIndexChanged);
            this.perfilesActualesListBox.MouseUp += new System.Windows.Forms.MouseEventHandler(this.perfilesActualesListBox_MouseUp);
            // 
            // perfilesContextMenuStrip
            // 
            this.perfilesContextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.nuevoPerfilToolStripMenuItem,
            this.editarPerfilToolStripMenuItem,
            this.eliminarPerfilToolStripMenuItem});
            this.perfilesContextMenuStrip.Name = "perfilesContextMenuStrip";
            this.perfilesContextMenuStrip.Size = new System.Drawing.Size(149, 70);
            // 
            // nuevoPerfilToolStripMenuItem
            // 
            this.nuevoPerfilToolStripMenuItem.Name = "nuevoPerfilToolStripMenuItem";
            this.nuevoPerfilToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.nuevoPerfilToolStripMenuItem.Text = "Nuevo perfil";
            this.nuevoPerfilToolStripMenuItem.Click += new System.EventHandler(this.nuevoPerfilButton_Click);
            // 
            // editarPerfilToolStripMenuItem
            // 
            this.editarPerfilToolStripMenuItem.Name = "editarPerfilToolStripMenuItem";
            this.editarPerfilToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.editarPerfilToolStripMenuItem.Text = "Editar perfil";
            this.editarPerfilToolStripMenuItem.Click += new System.EventHandler(this.editarPerfilButton_Click);
            // 
            // eliminarPerfilToolStripMenuItem
            // 
            this.eliminarPerfilToolStripMenuItem.Name = "eliminarPerfilToolStripMenuItem";
            this.eliminarPerfilToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.eliminarPerfilToolStripMenuItem.Text = "Eliminar perfil";
            this.eliminarPerfilToolStripMenuItem.Click += new System.EventHandler(this.eliminarPerfilButton_Click);
            // 
            // perfilesActualesLabel
            // 
            this.perfilesActualesLabel.AutoSize = true;
            this.perfilesActualesLabel.Location = new System.Drawing.Point(11, 13);
            this.perfilesActualesLabel.Name = "perfilesActualesLabel";
            this.perfilesActualesLabel.Size = new System.Drawing.Size(87, 13);
            this.perfilesActualesLabel.TabIndex = 1;
            this.perfilesActualesLabel.Text = "Perfiles actuales:";
            // 
            // aceptarButton
            // 
            this.aceptarButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.aceptarButton.Location = new System.Drawing.Point(302, 240);
            this.aceptarButton.Name = "aceptarButton";
            this.aceptarButton.Size = new System.Drawing.Size(75, 25);
            this.aceptarButton.TabIndex = 5;
            this.aceptarButton.Text = "Aceptar";
            this.aceptarButton.UseVisualStyleBackColor = true;
            this.aceptarButton.Click += new System.EventHandler(this.aceptarButton_Click);
            // 
            // eliminarPerfilButton
            // 
            this.eliminarPerfilButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.eliminarPerfilButton.Enabled = false;
            this.eliminarPerfilButton.Image = global::OCR.Properties.Resources.Eliminar;
            this.eliminarPerfilButton.Location = new System.Drawing.Point(302, 96);
            this.eliminarPerfilButton.Name = "eliminarPerfilButton";
            this.eliminarPerfilButton.Size = new System.Drawing.Size(75, 25);
            this.eliminarPerfilButton.TabIndex = 4;
            this.eliminarPerfilButton.Text = " Eliminar";
            this.eliminarPerfilButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.eliminarPerfilButton.UseVisualStyleBackColor = true;
            this.eliminarPerfilButton.Click += new System.EventHandler(this.eliminarPerfilButton_Click);
            // 
            // editarPerfilButton
            // 
            this.editarPerfilButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.editarPerfilButton.Enabled = false;
            this.editarPerfilButton.Image = global::OCR.Properties.Resources.Editar;
            this.editarPerfilButton.Location = new System.Drawing.Point(302, 67);
            this.editarPerfilButton.Name = "editarPerfilButton";
            this.editarPerfilButton.Size = new System.Drawing.Size(75, 25);
            this.editarPerfilButton.TabIndex = 3;
            this.editarPerfilButton.Text = " Editar";
            this.editarPerfilButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.editarPerfilButton.UseVisualStyleBackColor = true;
            this.editarPerfilButton.Click += new System.EventHandler(this.editarPerfilButton_Click);
            // 
            // nuevoPerfilButton
            // 
            this.nuevoPerfilButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nuevoPerfilButton.Image = global::OCR.Properties.Resources.Añadir;
            this.nuevoPerfilButton.Location = new System.Drawing.Point(302, 38);
            this.nuevoPerfilButton.Name = "nuevoPerfilButton";
            this.nuevoPerfilButton.Size = new System.Drawing.Size(75, 25);
            this.nuevoPerfilButton.TabIndex = 2;
            this.nuevoPerfilButton.Text = " Nuevo";
            this.nuevoPerfilButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.nuevoPerfilButton.UseVisualStyleBackColor = true;
            this.nuevoPerfilButton.Click += new System.EventHandler(this.nuevoPerfilButton_Click);
            // 
            // PerfilesForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(390, 279);
            this.Controls.Add(this.aceptarButton);
            this.Controls.Add(this.eliminarPerfilButton);
            this.Controls.Add(this.editarPerfilButton);
            this.Controls.Add(this.nuevoPerfilButton);
            this.Controls.Add(this.perfilesActualesLabel);
            this.Controls.Add(this.perfilesActualesListBox);
            this.Name = "PerfilesForm";
            this.Text = "Editar Perfiles";
            this.perfilesContextMenuStrip.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox perfilesActualesListBox;
        private System.Windows.Forms.Label perfilesActualesLabel;
        private System.Windows.Forms.Button nuevoPerfilButton;
        private System.Windows.Forms.Button editarPerfilButton;
        private System.Windows.Forms.Button eliminarPerfilButton;
        private System.Windows.Forms.Button aceptarButton;
        private System.Windows.Forms.ContextMenuStrip perfilesContextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem nuevoPerfilToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editarPerfilToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem eliminarPerfilToolStripMenuItem;
    }
}