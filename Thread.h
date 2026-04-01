#pragma once


namespace Thread {

	namespace EndThread {

		void EndSoftWare();
	}

	namespace AimBotThread {
		void AimBotMain();
	}


	namespace DrawThread {

		namespace Menu {

			void Menu();
		}

		namespace Function {
			void Function();
		}

		namespace Loop {
			void Loop();
		}

	}

	namespace GetData {
		void Thread_ReadView();
		void Thread_GetAddress();
		void Thread_ReadData();
		void Thread_ProcessingData();
	}


}
