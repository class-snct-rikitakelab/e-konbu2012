using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports; //for serial communicate


namespace LogTool
{
    public partial class LogToolForm : Form
    {
  
        //display accesible serial ports
        private void init()
        {
            // 選択可能なポートをSelBoxへ
            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports)
            {
                COMportsComboBox.Items.Add(port);
            }


        }
        private void connectBlueToothDevice()
        {
            //string PortName = "COM1";
           // string PortName = COMportsComboBox.Text;
            
            int BaudRate = 38400;
            Parity Parity = Parity.None;
            int DataBits = 8;
            StopBits StopBits = StopBits.One;

            SerialPort myPort =
            new SerialPort(PortName, BaudRate, Parity, DataBits, StopBits);
          
            try   	{			//	例外処理のはじまり 
   	　if(myPort.IsOpen == true){   		//ポートはオープン済み
	   //	MessageBox.Show("エラー", MessageBoxButtons.OK, MessageBoxIcon.Error)                
     }
         else{
	 	    string PortName = COMportsComboBox.Text; 	//オープンするポート名を格納
	   	    myPort.Open();			//ポートオープン        
     }
            catch (){  			//例外処理            
	  	    //MessageBox.Show(ex.Message, "エラー", MessageBoxButtons.OK,MessageBoxIcon.Error)        
            }
        }
     }
         //   myPort.Open(); 



        }
    }

}