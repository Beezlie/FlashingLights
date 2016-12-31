void DAC_Init(void);
void DAC_Out(unsigned long data);
void Timer2_Init(unsigned long period);
void Sound_Init(void);
void Timer2A_Stop(void);
void Timer2A_Start(void);
void Timer2A_Handler(void);
void Sound_Play(const unsigned char *pt, unsigned long count);