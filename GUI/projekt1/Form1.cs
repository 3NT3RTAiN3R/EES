using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace projekt1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        public void getServerData(string Message)
        {
            string zwischenData;

            Byte[] msg;
            msg = Encoding.ASCII.GetBytes(Message);
            var client = new UdpClient();
            IPEndPoint ep = new IPEndPoint(IPAddress.Parse("192.168.178.42"), 4030); // endpoint where server is listening
            client.Connect(ep);
            
            // send data
            client.Send(msg, msg.Length);

            // then receive data
            Byte[] receivedData = client.Receive(ref ep);
            if(Message == "GUI;Ultraschall;get") { 
                zwischenData = Encoding.ASCII.GetString(receivedData);
                string[] receivedMessage = zwischenData.Split(new char[1] {';'});
                //Console.Write("receive data from " + ep.ToString());
                // in 0/3/6/9 sind ID ... 1/4/7/10 current ... 2/5/8/11 empty
                lblOutSensorA.Text     = receivedMessage[1];
                lblOutSensorAInit.Text = receivedMessage[2];

                lblOutSensorB.Text     = receivedMessage[5];
                lblOutSensorBInit.Text = receivedMessage[4];

                lblOutSensor1.Text     = receivedMessage[8];
                lblOutSensor1Init.Text = receivedMessage[7];

                lblOutSensor2.Text     = receivedMessage[11];
                lblOutSensor2Init.Text = receivedMessage[10];
            }
            //Console.Read();
            //return new string[]{ ID, current, zusatz };

        }
        private void btnUpdateRoom_Click(object sender, EventArgs e)
        {
             getServerData("GUI;Ultraschall;get");
            //lblTest.Text = data[0];
        }

        private void btnGuertel_Click(object sender, EventArgs e)
        {
            getServerData("GUI;Guertel;off");
        }

        private void btnRoomClear_Click(object sender, EventArgs e)
        {
            lblOutSensorA.Text ="";
            lblOutSensorAInit.Text = "";

            lblOutSensorB.Text = "";
            lblOutSensorBInit.Text ="";

            lblOutSensor1.Text = "";
            lblOutSensor1Init.Text = "";

            lblOutSensor2.Text = "";
            lblOutSensor2Init.Text = "";
        }
    }
}
