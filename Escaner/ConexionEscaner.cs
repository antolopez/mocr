using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using TwainLib;

namespace Escaner
{
    public class ConexionEscaner
    {
        //Atributos
        private Twain escaner;
        private TwProtocol protocoloEscaner;
        private IntPtr handle;


        //M�todos

        //============================================================================
        // NOMBRE: ConexionEscaner
        //
        // DESCRIPCI�N: Constructor que inicializa los atributos para la gesti�n del esc�ner.
        //
        // ARGUMENTOS: IntPtr handle -> Identificador de ventana al que est� enlazado el control	
        //
        // SALIDA: -
        //============================================================================
        public ConexionEscaner(IntPtr handle)
        {
            escaner = new Twain();
            protocoloEscaner = new TwProtocol();
            this.handle = handle;
        }


        //============================================================================
        // NOMBRE: Inicializar
        //
        // DESCRIPCI�N: Prepara el esc�ner para su uso.
        //
        // ARGUMENTOS: -	
        //
        // SALIDA: Booleano que indica si la inicializaci�n del esc�ner se ha producido correctamente
        //============================================================================
        public bool Inicializar()
        {
            TwRC existeTwain = escaner.Init(handle);
            
            if (existeTwain == TwRC.Success)
                return true;//Existe escaner
            else
                return false;//No existe escaner o se ha producido alg�n error
        }


        //============================================================================
        // NOMBRE: SeleccionarEscaner
        //
        // DESCRIPCI�N: Permite seleccionar un esc�ner de entre todos los instalados en el sistema.
        //
        // ARGUMENTOS: -	
        //
        // SALIDA: Booleano que indica si la selecci�n del esc�ner se ha realizado correctamente
        //============================================================================
        public bool SeleccionarEscaner()
        {
            TwRC accionUsuario = escaner.Select();

            if (accionUsuario == TwRC.Success)
                return true;//Escaner seleccionado correctamente
            else
                return false;//El usuario ha cancelado la selecci�n o bien se ha producido alg�n error
        }


        //============================================================================
        // NOMBRE: Adquirir
        //
        // DESCRIPCI�N: Muestra la ventana de opciones del esc�ner.
        //
        // ARGUMENTOS: -	
        //
        // SALIDA: Booleano que indica si el esc�ner es accesible
        //============================================================================
        public bool Adquirir()
        {
            TwRC escanerAccesible = escaner.Acquire();

            if (escanerAccesible == TwRC.Success)
                return true;
            else
                return false;
        }


        //============================================================================
        // NOMBRE: TransferirImagenes
        //
        // DESCRIPCI�N: Ordena la transferencia de la im�gen/esMuestra la ventana de opciones del esc�ner.
        //
        // ARGUMENTOS: -	
        //
        // SALIDA: ArrayList que contiene todas las im�genes escaneadas
        //============================================================================
        public ArrayList TransferirImagenes()
        {
            return escaner.TransferPictures();
        }


        //============================================================================
        // NOMBRE: RecibirMensaje
        //
        // DESCRIPCI�N: Recibe un mensaje devolviendo un comando.
        //
        // ARGUMENTOS: -	
        //
        // SALIDA: Comando realizado/a realizar
        //============================================================================
        public TwainCommand RecibirMensaje(ref Message m)
        {
            return escaner.PassMessage(ref m);
        }


        //============================================================================
        // NOMBRE: CerrarConexion
        //
        // DESCRIPCI�N: Cierra la conexi�n con el esc�ner (al terminar una transacci�n). 
        //
        // ARGUMENTOS: -	
        //
        // SALIDA: -
        //============================================================================
        public void CerrarConexion()
        {
            escaner.CloseSrc();
        }


        //============================================================================
        // NOMBRE: Finalizar
        //
        // DESCRIPCI�N: Elimina cualquier referencia al esc�ner(cuando ya no lo vamos a usar).
        //
        // ARGUMENTOS: -	
        //
        // SALIDA: -
        //============================================================================
        public void Finalizar()
        {
            escaner.Finish();
        }    
        
    }
}
