// ---------------------------------------------------------------------------
//	OPM-like Sound Generator
//	Copyright (C) cisc 1998, 2003.
// ---------------------------------------------------------------------------
//	$fmgen-Id: opm.h,v 1.14 2003/06/07 08:25:53 cisc Exp $

#ifndef FM_OPM_H
#define FM_OPM_H

#include "fmgen.h"
#include "fmtimer.h"
#include "psg.h"

// ---------------------------------------------------------------------------
//	class OPM
//	OPM ���ɤ�����(?)�����������벻����˥å�
//	
//	interface:
//	bool Init(uint clock, uint rate, bool);
//		����������Υ��饹����Ѥ������ˤ��ʤ餺�Ƥ�Ǥ������ȡ�
//		����: �����䴰�⡼�ɤ��ѻߤ���ޤ���
//
//		clock:	OPM �Υ����å����ȿ�(Hz)
//
//		rate:	�������� PCM ��ɸ�ܼ��ȿ�(Hz)
//
//				
//		����	���������������� true
//
//	bool SetRate(uint clock, uint rate, bool)
//		�����å��� PCM �졼�Ȥ��ѹ�����
//		�������� Init ��Ʊ�͡�
//	
//	void Mix(Sample* dest, int nsamples)
//		Stereo PCM �ǡ����� nsamples ʬ�������� dest �ǻϤޤ������
//		�ä���(�û�����)
//		��dest �ˤ� sample*2 ��ʬ���ΰ褬ɬ��
//		����Ǽ������ L, R, L, R... �Ȥʤ롥
//		�������ޤǲû��ʤΤǡ����餫��������򥼥����ꥢ����ɬ�פ�����
//		��FM_SAMPLETYPE �� short ���ξ�祯��åԥ󥰤��Ԥ���.
//		�����δؿ��ϲ��������Υ����ޡ��Ȥ���Ω���Ƥ��롥
//		  Timer �� Count �� GetNextEvent ������ɬ�פ����롥
//	
//	void Reset()
//		������ꥻ�å�(�����)����
//
//	void SetReg(uint reg, uint data)
//		�����Υ쥸���� reg �� data ��񤭹���
//	
//	uint ReadStatus()
//		�����Υ��ơ������쥸�������ɤ߽Ф�
//		busy �ե饰�Ͼ�� 0
//	
//	bool Count(uint32 t)
//		�����Υ����ޡ��� t [10^(-6) ��] �ʤ�롥
//		�������������֤��Ѳ������ä���(timer �����С��ե���)
//		true ���֤�
//
//	uint32 GetNextEvent()
//		�����Υ����ޡ��Τɤ��餫�������С��ե�������ޤǤ�ɬ�פ�
//		����[����]���֤�
//		�����ޡ�����ߤ��Ƥ������ 0 ���֤���
//	
//	void SetVolume(int db)
//		�Ʋ����β��̤�ܡ�������Ĵ�᤹�롥ɸ���ͤ� 0.
//		ñ�̤��� 1/2 dB��ͭ���ϰϤξ�¤� 20 (10dB)
//
//	���۴ؿ�:
//	virtual void Intr(bool irq)
//		IRQ ���Ϥ��Ѳ������ä����ƤФ�롥
//		irq = true:  IRQ �׵᤬ȯ��
//		irq = false: IRQ �׵᤬�ä���
//
namespace FM
{
	//	YM2151(OPM) ----------------------------------------------------
	class OPM : public Timer
	{
	public:
		OPM();
		virtual ~OPM() {}

		bool	Init(uint c, uint r, bool=false);
		bool	SetRate(uint c, uint r, bool);
		void	SetLPFCutoff(uint freq);
		void	Reset();
		
		void 	SetReg(uint addr, uint data);
		uint	GetReg(uint addr);
		uint	ReadStatus() { return status & 0x03; }
		
		void 	Mix(Sample* buffer, int nsamples);
		
		void	SetVolume(int db);
		void	SetChannelMask(uint mask);
		
	private:
		virtual void Intr(bool) {}
	
	private:
		enum
		{
			OPM_LFOENTS = 512,
		};
		
		void	SetStatus(uint bit);
		void	ResetStatus(uint bit);
		void	SetParameter(uint addr, uint data);
		void	TimerA();
		void	RebuildTimeTable();
		void	MixSub(int activech, ISample**);
		void	MixSubL(int activech, ISample**);
		void	LFO();
		uint	Noise();
		
		int		fmvolume;

		uint	clock;
		uint	rate;
		uint	pcmrate;

		uint	pmd;
		uint	amd;
		uint	lfocount;
		uint	lfodcount;

		uint	lfo_count_;
		uint	lfo_count_diff_;
		uint	lfo_step_;
		uint	lfo_count_prev_;

		uint	lfowaveform;
		uint	rateratio;
		uint	noise;
		int32	noisecount;
		uint32	noisedelta;
		
		bool	interpolation;
		uint8	lfofreq;
		uint8	status;
		uint8	reg01;

		uint8	kc[8];
		uint8	kf[8];
		uint8	pan[8];

		Channel4 ch[8];
		Chip	chip;

		static void	BuildLFOTable();
		static int amtable[4][OPM_LFOENTS];
		static int pmtable[4][OPM_LFOENTS];

	public:
		int		dbgGetOpOut(int c, int s) { return ch[c].op[s].dbgopout_; }
		Channel4* dbgGetCh(int c) { return &ch[c]; }

	};
}

#endif // FM_OPM_H