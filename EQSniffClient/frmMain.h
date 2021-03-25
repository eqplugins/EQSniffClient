#pragma once
#include "PracticalSocket.h"
using namespace System::Threading;

const int ECHOMAX = 5000;
const int OFFSETTYPE = 1;
const int OFFSETADDRESS = 5;
const int OFFSETPACKET = 9;
const int PORT=54540;
typedef unsigned long DWORD;
int packet = 0;
bool pause = false;
namespace EQSniffClient {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for frmMain
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmMain : public System::Windows::Forms::Form
	{
	public:
		frmMain(void)
		{
			InitializeComponent();
			SpawnThread();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmMain()
		{
			if (components)
			{
				delete components;
			}
		}
	protected:
		delegate void FormDelegate(System::Object ^obj);
	protected:
		void changeIN(char* Packet, DWORD& Type, DWORD Size)
		{
			frmMain::tbINType->Text=Type.ToString("X");
			frmMain::tbINSize->Text=Size.ToString();
			frmMain::tbIN->Text=gcnew String(Packet);
		}
	protected:
		void changeOUT(char* Packet, DWORD& Type, DWORD Size, DWORD& Address)
		{
			frmMain::tbOUTType->Text=Type.ToString("X");
			frmMain::tbOUTSize->Text=Size.ToString();
			frmMain::tbOUTAddress->Text=Address.ToString("X");
			frmMain::tbOUT->Text=gcnew String(Packet);
		}
	protected:
		void changePause()
		{
			if (pause) frmMain::btnPause->Text="Resume";
			else frmMain::btnPause->Text="Pause";
		}

	private: System::Windows::Forms::TextBox^  tbFilterPacket;
	private: System::Windows::Forms::Button^  btnFilter;
	private: System::Windows::Forms::TextBox^  tbOUTAddress;
	private: System::Windows::Forms::Button^  btnClear;
	private: System::Windows::Forms::TextBox^  tbINTypeList;
	private: System::Windows::Forms::TextBox^  tbOUTSize;
	private: System::Windows::Forms::TextBox^  tbOUTType;
	private: System::Windows::Forms::TextBox^  tbINSize;
	private: System::Windows::Forms::TextBox^  tbINType;
	private: System::Windows::Forms::Button^  btnPause;
	private: System::Windows::Forms::TextBox^  tbOUT;
	private: System::Windows::Forms::TextBox^  tbIN;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		System::ComponentModel::Container ^components;

		delegate void DelegateThreadTask();

		void SpawnThread() {
			   Thread^ trd = gcnew Thread( gcnew ParameterizedThreadStart( &ThreadTask ) );
				trd->IsBackground = true;
				if (!trd->IsAlive) trd->Start(this);
		}

		static void ThreadTask(System::Object ^obj)
		{
			frmMain ^ob = (frmMain^) obj;
			if (ob->InvokeRequired)
			{
				FormDelegate ^d = gcnew FormDelegate(ThreadTask);
				ob->Invoke(d,gcnew array<System::Object^>{ob});				
			} else {
			unsigned short echoServPort = PORT;
			UDPSocket sock(echoServPort);                
  
			char echoBuffer[ECHOMAX];
			char finalhash[ECHOMAX*2];

			int recvMsgSize;
			string sourceAddress;
			unsigned short sourcePort;
			char hexval[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
			while(true){
				recvMsgSize = sock.recvFrom(echoBuffer, ECHOMAX, sourceAddress, sourcePort);				
				for(int j = 0; j < recvMsgSize-OFFSETPACKET; j++)
				{
					finalhash[j*2] = hexval[((echoBuffer[OFFSETPACKET+j] >> 4) & 0xF)];
					finalhash[(j*2) + 1] = hexval[(echoBuffer[OFFSETPACKET+j]) & 0x0F];
				}
				finalhash[(recvMsgSize-OFFSETPACKET)*2]='\0';
				if (!pause && !packet) {
					//if ((DWORD&)(echoBuffer+OFFSETTYPE)[0]==(DWORD)packet) {
					if (echoBuffer[0]==1) {
						ob->changeIN(finalhash,(DWORD&)(echoBuffer+OFFSETTYPE)[0],recvMsgSize-OFFSETPACKET); 
					} else {
						ob->changeOUT(finalhash,(DWORD&)(echoBuffer+OFFSETTYPE)[0],recvMsgSize-OFFSETPACKET,(DWORD&)(echoBuffer+OFFSETADDRESS)[0]);
					}
					//}
				}
			}
			}
		}

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->tbFilterPacket = (gcnew System::Windows::Forms::TextBox());
			this->btnFilter = (gcnew System::Windows::Forms::Button());
			this->tbOUTAddress = (gcnew System::Windows::Forms::TextBox());
			this->btnClear = (gcnew System::Windows::Forms::Button());
			this->tbINTypeList = (gcnew System::Windows::Forms::TextBox());
			this->tbOUTSize = (gcnew System::Windows::Forms::TextBox());
			this->tbOUTType = (gcnew System::Windows::Forms::TextBox());
			this->tbINSize = (gcnew System::Windows::Forms::TextBox());
			this->tbINType = (gcnew System::Windows::Forms::TextBox());
			this->btnPause = (gcnew System::Windows::Forms::Button());
			this->tbOUT = (gcnew System::Windows::Forms::TextBox());
			this->tbIN = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// tbFilterPacket
			// 
			this->tbFilterPacket->Location = System::Drawing::Point(715, 37);
			this->tbFilterPacket->Name = L"tbFilterPacket";
			this->tbFilterPacket->Size = System::Drawing::Size(72, 20);
			this->tbFilterPacket->TabIndex = 24;
			// 
			// btnFilter
			// 
			this->btnFilter->Location = System::Drawing::Point(715, 8);
			this->btnFilter->Name = L"btnFilter";
			this->btnFilter->Size = System::Drawing::Size(75, 23);
			this->btnFilter->TabIndex = 23;
			this->btnFilter->Text = L"Filter";
			this->btnFilter->UseVisualStyleBackColor = true;
			// 
			// tbOUTAddress
			// 
			this->tbOUTAddress->Location = System::Drawing::Point(242, 320);
			this->tbOUTAddress->Name = L"tbOUTAddress";
			this->tbOUTAddress->Size = System::Drawing::Size(80, 20);
			this->tbOUTAddress->TabIndex = 22;
			// 
			// btnClear
			// 
			this->btnClear->Location = System::Drawing::Point(146, 8);
			this->btnClear->Name = L"btnClear";
			this->btnClear->Size = System::Drawing::Size(88, 48);
			this->btnClear->TabIndex = 21;
			this->btnClear->Text = L"Clear";
			this->btnClear->Click += gcnew System::EventHandler(this, &frmMain::btnClear_Click);
			// 
			// tbINTypeList
			// 
			this->tbINTypeList->Location = System::Drawing::Point(453, 346);
			this->tbINTypeList->Multiline = true;
			this->tbINTypeList->Name = L"tbINTypeList";
			this->tbINTypeList->ScrollBars = System::Windows::Forms::ScrollBars::Horizontal;
			this->tbINTypeList->Size = System::Drawing::Size(337, 216);
			this->tbINTypeList->TabIndex = 20;
			// 
			// tbOUTSize
			// 
			this->tbOUTSize->Location = System::Drawing::Point(146, 320);
			this->tbOUTSize->Name = L"tbOUTSize";
			this->tbOUTSize->Size = System::Drawing::Size(80, 20);
			this->tbOUTSize->TabIndex = 19;
			// 
			// tbOUTType
			// 
			this->tbOUTType->Location = System::Drawing::Point(50, 320);
			this->tbOUTType->Name = L"tbOUTType";
			this->tbOUTType->Size = System::Drawing::Size(80, 20);
			this->tbOUTType->TabIndex = 18;
			// 
			// tbINSize
			// 
			this->tbINSize->Location = System::Drawing::Point(146, 72);
			this->tbINSize->Name = L"tbINSize";
			this->tbINSize->Size = System::Drawing::Size(80, 20);
			this->tbINSize->TabIndex = 17;
			// 
			// tbINType
			// 
			this->tbINType->Location = System::Drawing::Point(50, 72);
			this->tbINType->Name = L"tbINType";
			this->tbINType->Size = System::Drawing::Size(80, 20);
			this->tbINType->TabIndex = 16;
			// 
			// btnPause
			// 
			this->btnPause->Location = System::Drawing::Point(50, 8);
			this->btnPause->Name = L"btnPause";
			this->btnPause->Size = System::Drawing::Size(88, 48);
			this->btnPause->TabIndex = 15;
			this->btnPause->Text = L"Pause";
			this->btnPause->Click += gcnew System::EventHandler(this, &frmMain::btnPause_Click);
			// 
			// tbOUT
			// 
			this->tbOUT->Location = System::Drawing::Point(50, 346);
			this->tbOUT->Multiline = true;
			this->tbOUT->Name = L"tbOUT";
			this->tbOUT->Size = System::Drawing::Size(397, 216);
			this->tbOUT->TabIndex = 14;
			// 
			// tbIN
			// 
			this->tbIN->Location = System::Drawing::Point(50, 96);
			this->tbIN->Multiline = true;
			this->tbIN->Name = L"tbIN";
			this->tbIN->Size = System::Drawing::Size(397, 200);
			this->tbIN->TabIndex = 13;
			// 
			// frmMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(841, 570);
			this->Controls->Add(this->tbFilterPacket);
			this->Controls->Add(this->btnFilter);
			this->Controls->Add(this->tbOUTAddress);
			this->Controls->Add(this->btnClear);
			this->Controls->Add(this->tbINTypeList);
			this->Controls->Add(this->tbOUTSize);
			this->Controls->Add(this->tbOUTType);
			this->Controls->Add(this->tbINSize);
			this->Controls->Add(this->tbINType);
			this->Controls->Add(this->btnPause);
			this->Controls->Add(this->tbOUT);
			this->Controls->Add(this->tbIN);
			this->Name = L"frmMain";
			this->Text = L"SniffClient";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btnPause_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (pause) {
						pause=false;
						changePause();
				}
				 else {
						pause=true;
						changePause();
				}
			 }
private: System::Void btnClear_Click(System::Object^  sender, System::EventArgs^  e) {
			 	this->tbIN->Text="";
				this->tbINSize->Text="";
				this->tbINType->Text="";
				this->tbOUT->Text="";
				this->tbOUTSize->Text="";
				this->tbOUTType->Text="";
		 }
};
}

