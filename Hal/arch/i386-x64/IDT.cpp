/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: Hal
File: IDT.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\Hal\arch\i386-x64\IDT.cpp
Created by Nathaniel on 13/7/2015 at 12:21

Description: Interrupt Descriptor Table
**********************************************************/

#include "IDT.h"
#include "GDT.h"
#include "InterruptManager.h"
#include <memory.h>
#include <string.h>
#include "idtasm.h"

extern "C" void __fastcall interruptHandler(size_t intNum, size_t* param)
{
	ALIGN(16,char floatstate[512]);
	FXSAVE(floatstate);
	if (getHal()->getInterruptManager()->readParam(intNum, 1) != -1)
		getHal()->getInterruptManager()->callInterrupt(intNum);
	else
		getHal()->getInterruptManager()->callInterrupt(intNum, param);
	FXRSTOR(floatstate);
}


void CALLING_CONVENTION CIDT::initialise()
{
	idt = new IDTENT[IDTSIZE];
	memset(idt, 0, sizeof(IDTENT[IDTSIZE]));
	//Create Register
	IDTDESC idtr;
	idtr.size = sizeof(IDTENT[IDTSIZE]) - 1;
	idtr.offset = (size_t)idt;

#if 0
#endif
#if 1
	setEntry(0, &intrptHndlr0);
	setEntry(1, &intrptHndlr1);
	setEntry(2, &intrptHndlr2);
	setEntry(3, &intrptHndlr3);
	setEntry(4, &intrptHndlr4);
	setEntry(5, &intrptHndlr5);
	setEntry(6, &intrptHndlr6);
	setEntry(7, &intrptHndlr7);
	setEntry(8, &intrptHndlr8);
	setEntry(9, &intrptHndlr9);
	setEntry(10, &intrptHndlr10);
	setEntry(11, &intrptHndlr11);
	setEntry(12, &intrptHndlr12);
	setEntry(13, &intrptHndlr13);
	setEntry(14, &intrptHndlr14);
	setEntry(15, &intrptHndlr15);
	setEntry(16, &intrptHndlr16);
	setEntry(17, &intrptHndlr17);
	setEntry(18, &intrptHndlr18);
	setEntry(19, &intrptHndlr19);
	setEntry(20, &intrptHndlr20);
	setEntry(21, &intrptHndlr21);
	setEntry(22, &intrptHndlr22);
	setEntry(23, &intrptHndlr23);
	setEntry(24, &intrptHndlr24);
	setEntry(25, &intrptHndlr25);
	setEntry(26, &intrptHndlr26);
	setEntry(27, &intrptHndlr27);
	setEntry(28, &intrptHndlr28);
	setEntry(29, &intrptHndlr29);
	setEntry(30, &intrptHndlr30);
	setEntry(31, &intrptHndlr31);
	setEntry(32, &intrptHndlr32);
	setEntry(33, &intrptHndlr33);
	setEntry(34, &intrptHndlr34);
	setEntry(35, &intrptHndlr35);
	setEntry(36, &intrptHndlr36);
	setEntry(37, &intrptHndlr37);
	setEntry(38, &intrptHndlr38);
	setEntry(39, &intrptHndlr39);
	setEntry(40, &intrptHndlr40);
	setEntry(41, &intrptHndlr41);
	setEntry(42, &intrptHndlr42);
	setEntry(43, &intrptHndlr43);
	setEntry(44, &intrptHndlr44);
	setEntry(45, &intrptHndlr45);
	setEntry(46, &intrptHndlr46);
	setEntry(47, &intrptHndlr47);
	setEntry(48, &intrptHndlr48);
	setEntry(49, &intrptHndlr49);
	setEntry(50, &intrptHndlr50);
	setEntry(51, &intrptHndlr51);
	setEntry(52, &intrptHndlr52);
	setEntry(53, &intrptHndlr53);
	setEntry(54, &intrptHndlr54);
	setEntry(55, &intrptHndlr55);
	setEntry(56, &intrptHndlr56);
	setEntry(57, &intrptHndlr57);
	setEntry(58, &intrptHndlr58);
	setEntry(59, &intrptHndlr59);
	setEntry(60, &intrptHndlr60);
	setEntry(61, &intrptHndlr61);
	setEntry(62, &intrptHndlr62);
	setEntry(63, &intrptHndlr63);
	setEntry(64, &intrptHndlr64);
	setEntry(65, &intrptHndlr65);
	setEntry(66, &intrptHndlr66);
	setEntry(67, &intrptHndlr67);
	setEntry(68, &intrptHndlr68);
	setEntry(69, &intrptHndlr69);
	setEntry(70, &intrptHndlr70);
	setEntry(71, &intrptHndlr71);
	setEntry(72, &intrptHndlr72);
	setEntry(73, &intrptHndlr73);
	setEntry(74, &intrptHndlr74);
	setEntry(75, &intrptHndlr75);
	setEntry(76, &intrptHndlr76);
	setEntry(77, &intrptHndlr77);
	setEntry(78, &intrptHndlr78);
	setEntry(79, &intrptHndlr79);
	setEntry(80, &intrptHndlr80);
	setEntry(81, &intrptHndlr81);
	setEntry(82, &intrptHndlr82);
	setEntry(83, &intrptHndlr83);
	setEntry(84, &intrptHndlr84);
	setEntry(85, &intrptHndlr85);
	setEntry(86, &intrptHndlr86);
	setEntry(87, &intrptHndlr87);
	setEntry(88, &intrptHndlr88);
	setEntry(89, &intrptHndlr89);
	setEntry(90, &intrptHndlr90);
	setEntry(91, &intrptHndlr91);
	setEntry(92, &intrptHndlr92);
	setEntry(93, &intrptHndlr93);
	setEntry(94, &intrptHndlr94);
	setEntry(95, &intrptHndlr95);
	setEntry(96, &intrptHndlr96);
	setEntry(97, &intrptHndlr97);
	setEntry(98, &intrptHndlr98);
	setEntry(99, &intrptHndlr99);
	setEntry(100, &intrptHndlr100);
	setEntry(101, &intrptHndlr101);
	setEntry(102, &intrptHndlr102);
	setEntry(103, &intrptHndlr103);
	setEntry(104, &intrptHndlr104);
	setEntry(105, &intrptHndlr105);
	setEntry(106, &intrptHndlr106);
	setEntry(107, &intrptHndlr107);
	setEntry(108, &intrptHndlr108);
	setEntry(109, &intrptHndlr109);
	setEntry(110, &intrptHndlr110);
	setEntry(111, &intrptHndlr111);
	setEntry(112, &intrptHndlr112);
	setEntry(113, &intrptHndlr113);
	setEntry(114, &intrptHndlr114);
	setEntry(115, &intrptHndlr115);
	setEntry(116, &intrptHndlr116);
	setEntry(117, &intrptHndlr117);
	setEntry(118, &intrptHndlr118);
	setEntry(119, &intrptHndlr119);
	setEntry(120, &intrptHndlr120);
	setEntry(121, &intrptHndlr121);
	setEntry(122, &intrptHndlr122);
	setEntry(123, &intrptHndlr123);
	setEntry(124, &intrptHndlr124);
	setEntry(125, &intrptHndlr125);
	setEntry(126, &intrptHndlr126);
	setEntry(127, &intrptHndlr127);
	setEntry(128, &intrptHndlr128);
	setEntry(129, &intrptHndlr129);
	setEntry(130, &intrptHndlr130);
	setEntry(131, &intrptHndlr131);
	setEntry(132, &intrptHndlr132);
	setEntry(133, &intrptHndlr133);
	setEntry(134, &intrptHndlr134);
	setEntry(135, &intrptHndlr135);
	setEntry(136, &intrptHndlr136);
	setEntry(137, &intrptHndlr137);
	setEntry(138, &intrptHndlr138);
	setEntry(139, &intrptHndlr139);
	setEntry(140, &intrptHndlr140);
	setEntry(141, &intrptHndlr141);
	setEntry(142, &intrptHndlr142);
	setEntry(143, &intrptHndlr143);
	setEntry(144, &intrptHndlr144);
	setEntry(145, &intrptHndlr145);
	setEntry(146, &intrptHndlr146);
	setEntry(147, &intrptHndlr147);
	setEntry(148, &intrptHndlr148);
	setEntry(149, &intrptHndlr149);
	setEntry(150, &intrptHndlr150);
	setEntry(151, &intrptHndlr151);
	setEntry(152, &intrptHndlr152);
	setEntry(153, &intrptHndlr153);
	setEntry(154, &intrptHndlr154);
	setEntry(155, &intrptHndlr155);
	setEntry(156, &intrptHndlr156);
	setEntry(157, &intrptHndlr157);
	setEntry(158, &intrptHndlr158);
	setEntry(159, &intrptHndlr159);
	setEntry(160, &intrptHndlr160);
	setEntry(161, &intrptHndlr161);
	setEntry(162, &intrptHndlr162);
	setEntry(163, &intrptHndlr163);
	setEntry(164, &intrptHndlr164);
	setEntry(165, &intrptHndlr165);
	setEntry(166, &intrptHndlr166);
	setEntry(167, &intrptHndlr167);
	setEntry(168, &intrptHndlr168);
	setEntry(169, &intrptHndlr169);
	setEntry(170, &intrptHndlr170);
	setEntry(171, &intrptHndlr171);
	setEntry(172, &intrptHndlr172);
	setEntry(173, &intrptHndlr173);
	setEntry(174, &intrptHndlr174);
	setEntry(175, &intrptHndlr175);
	setEntry(176, &intrptHndlr176);
	setEntry(177, &intrptHndlr177);
	setEntry(178, &intrptHndlr178);
	setEntry(179, &intrptHndlr179);
	setEntry(180, &intrptHndlr180);
	setEntry(181, &intrptHndlr181);
	setEntry(182, &intrptHndlr182);
	setEntry(183, &intrptHndlr183);
	setEntry(184, &intrptHndlr184);
	setEntry(185, &intrptHndlr185);
	setEntry(186, &intrptHndlr186);
	setEntry(187, &intrptHndlr187);
	setEntry(188, &intrptHndlr188);
	setEntry(189, &intrptHndlr189);
	setEntry(190, &intrptHndlr190);
	setEntry(191, &intrptHndlr191);
	setEntry(192, &intrptHndlr192);
	setEntry(193, &intrptHndlr193);
	setEntry(194, &intrptHndlr194);
	setEntry(195, &intrptHndlr195);
	setEntry(196, &intrptHndlr196);
	setEntry(197, &intrptHndlr197);
	setEntry(198, &intrptHndlr198);
	setEntry(199, &intrptHndlr199);
	setEntry(200, &intrptHndlr200);
	setEntry(201, &intrptHndlr201);
	setEntry(202, &intrptHndlr202);
	setEntry(203, &intrptHndlr203);
	setEntry(204, &intrptHndlr204);
	setEntry(205, &intrptHndlr205);
	setEntry(206, &intrptHndlr206);
	setEntry(207, &intrptHndlr207);
	setEntry(208, &intrptHndlr208);
	setEntry(209, &intrptHndlr209);
	setEntry(210, &intrptHndlr210);
	setEntry(211, &intrptHndlr211);
	setEntry(212, &intrptHndlr212);
	setEntry(213, &intrptHndlr213);
	setEntry(214, &intrptHndlr214);
	setEntry(215, &intrptHndlr215);
	setEntry(216, &intrptHndlr216);
	setEntry(217, &intrptHndlr217);
	setEntry(218, &intrptHndlr218);
	setEntry(219, &intrptHndlr219);
	setEntry(220, &intrptHndlr220);
	setEntry(221, &intrptHndlr221);
	setEntry(222, &intrptHndlr222);
	setEntry(223, &intrptHndlr223);
	setEntry(224, &intrptHndlr224);
	setEntry(225, &intrptHndlr225);
	setEntry(226, &intrptHndlr226);
	setEntry(227, &intrptHndlr227);
	setEntry(228, &intrptHndlr228);
	setEntry(229, &intrptHndlr229);
	setEntry(230, &intrptHndlr230);
	setEntry(231, &intrptHndlr231);
	setEntry(232, &intrptHndlr232);
	setEntry(233, &intrptHndlr233);
	setEntry(234, &intrptHndlr234);
	setEntry(235, &intrptHndlr235);
	setEntry(236, &intrptHndlr236);
	setEntry(237, &intrptHndlr237);
	setEntry(238, &intrptHndlr238);
	setEntry(239, &intrptHndlr239);
	setEntry(240, &intrptHndlr240);
	setEntry(241, &intrptHndlr241);
	setEntry(242, &intrptHndlr242);
	setEntry(243, &intrptHndlr243);
	setEntry(244, &intrptHndlr244);
	setEntry(245, &intrptHndlr245);
	setEntry(246, &intrptHndlr246);
	setEntry(247, &intrptHndlr247);
	setEntry(248, &intrptHndlr248);
	setEntry(249, &intrptHndlr249);
	setEntry(250, &intrptHndlr250);
	setEntry(251, &intrptHndlr251);
	setEntry(252, &intrptHndlr252);
	setEntry(253, &intrptHndlr253);
	setEntry(254, &intrptHndlr254);
	setEntry(255, &intrptHndlr255);
#endif
	
	LIDT(&idtr);
}

void CALLING_CONVENTION CIDT::setEntryPermissions(unsigned int entry, bool userAcessible)
{
	if (userAcessible)
		idt[entry].typeAttr.DPL = 3;
	else
		idt[entry].typeAttr.DPL = 0;
}

void CALLING_CONVENTION CIDT::setEntry(unsigned int entry, void* func)
{
	CGDT* gdt = static_cast<CGDT*>(getHal()->getSegmentTable());
	idt[entry].selector = gdt->getKernelCodeSelector();
	idt[entry].reserved = 0;
#ifdef X64
	//Define an IST for unusual events (these ae the ones the linux kernel uses. https://github.com/torvalds/linux/blob/master/Documentation/x86/kernel-stacks
	if (entry == 0x8 || entry == 0x2 || entry == 0x1 || entry == 0x3 || entry == 0x12)
	{
		idt[entry].intStackTable = gdt->x64getIST();
	}
	idt[entry].intStackTable = 0;
#endif
	idt[entry].typeAttr.type = interrupt_32;
	idt[entry].typeAttr.DPL = 3;
	idt[entry].typeAttr.present = 1;
	idt[entry].typeAttr.storage = 0;

	size_t fn = (size_t)func;
	idt[entry].offset1 = fn;
	idt[entry].offset2 = (fn>>16);
#ifdef X64
	idt[entry].offset3 = (fn>>32);
	idt[entry].reserved2 = 0;
#endif
}

