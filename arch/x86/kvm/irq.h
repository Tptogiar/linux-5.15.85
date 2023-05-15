/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * irq.h: in kernel interrupt controller related definitions
 * Copyright (c) 2007, Intel Corporation.
 *
 * Authors:
 *   Yaozu (Eddie) Dong <Eddie.dong@intel.com>
 */

#ifndef __IRQ_H
#define __IRQ_H

#include <linux/mm_types.h>
#include <linux/hrtimer.h>
#include <linux/kvm_host.h>
#include <linux/spinlock.h>

#include <kvm/iodev.h>
#include "lapic.h"

#define PIC_NUM_PINS 16
#define SELECT_PIC(irq) \
	((irq) < 8 ? KVM_IRQCHIP_PIC_MASTER : KVM_IRQCHIP_PIC_SLAVE)

struct kvm;
struct kvm_vcpu;

/* 对8259A状态的抽象 */
struct kvm_kpic_state {
	u8 last_irr;	/* edge detection */
	u8 irr;		/* interrupt request register */
	u8 imr;		/* interrupt mask register */
	u8 isr;		/* interrupt service register */
	u8 priority_add;	/* highest irq priority */
	u8 irq_base;
	u8 read_reg_select;
	u8 poll;
	u8 special_mask;
	u8 init_state;
	u8 auto_eoi;
	u8 rotate_on_auto_eoi;
	u8 special_fully_nested_mode;
	u8 init4;		/* true if 4 byte init */
	u8 elcr;		/* PIIX edge/trigger selection */
	u8 elcr_mask;
	u8 isr_ack;	/* interrupt ack detection */
	struct kvm_pic *pics_state;
};

/* 对8259A的抽象 */
struct kvm_pic {
	spinlock_t lock;
	bool wakeup_needed;
	unsigned pending_acks;
	struct kvm *kvm;
	struct kvm_kpic_state pics[2]; /* 0 is master pic, 1 is slave pic */
	int output;		/* intr from master PIC */
	struct kvm_io_device dev_master;
	struct kvm_io_device dev_slave;
	struct kvm_io_device dev_elcr;
	unsigned long irq_states[PIC_NUM_PINS];
};

//for_read_code int kvm_pic_init(struct kvm *kvm);
void kvm_pic_destroy(struct kvm *kvm);
int kvm_pic_read_irq(struct kvm *kvm);
void kvm_pic_update_irq(struct kvm_pic *s);

/* caller kvm_cpu_has_extint &
 *        kvm_cpu_get_extint &
 *        kvm_set_routing_entry &
 *        kvm_arch_post_irq_routing_update &
 *        kvm_ioapic_send_eoi &
 *        vcpu_scan_ioapic
 */
static inline int irqchip_split(struct kvm *kvm)
{
	int mode = kvm->arch.irqchip_mode;

	/* Matches smp_wmb() when setting irqchip_mode */
	smp_rmb();
	return mode == KVM_IRQCHIP_SPLIT;
}

/* caller pic_in_kernel &
 * 		  ioapic_in_kernel &
 * 		  kvm_arch_irqfd_allowed &
 * 		  kvm_free_irq_source_id *
 * 		  kvm_arch_vm_ioctl  KVM_SET_IRQCHIP KVM_GET_IRQCHIP   &
 * 		  kvm_irqchip_mode
 * 		  
 * 用来判断内核是否为这个vm创建过IRQchip了
 */
static inline int irqchip_kernel(struct kvm *kvm)
{
	int mode = kvm->arch.irqchip_mode;

	/* Matches smp_wmb() when setting irqchip_mode */
	smp_rmb();
	return mode == KVM_IRQCHIP_KERNEL;
}

/* caller kvm_vcpu_ioctl_interrupt &
 * 		  post_kvm_run_save &
 * 		  dm_request_for_irq_injection
 * 
 */
static inline int pic_in_kernel(struct kvm *kvm)
{
	return irqchip_kernel(kvm);
}

/* caller kvm_arch_irqfd_allowed &
 *        kvm_arch_can_set_irq_routing &
 *        kvm_vcpu_ioctl_interrupt &
 *        kvm_vcpu_ioctl_enable_cap &
 *        kvm_vm_ioctl_irq_line &
 *        kvm_vm_ioctl_enable_cap &
 *        kvm_arch_vm_ioctl &
 *        kvm_arch_vcpu_create &
 *        
 *        kvm_send_userspace_msi &
 *        
 *        avic_init_vcpu &
 *        svm_create_vcpu &
 *        
 *        vmx_can_use_vtd_pi
 * 
 * 用来判断内核是否为这个vm创建过IRQchip了 kvm_arch_vm_ioctl KVM_CREATE_IRQCHIP
 */
static inline int irqchip_in_kernel(struct kvm *kvm)
{
	int mode = kvm->arch.irqchip_mode;

	/* Matches smp_wmb() when setting irqchip_mode */
	smp_rmb();
	return mode != KVM_IRQCHIP_NONE;
}

void kvm_inject_pending_timer_irqs(struct kvm_vcpu *vcpu);
void kvm_inject_apic_timer_irqs(struct kvm_vcpu *vcpu);
void kvm_apic_nmi_wd_deliver(struct kvm_vcpu *vcpu);
void __kvm_migrate_apic_timer(struct kvm_vcpu *vcpu);
void __kvm_migrate_pit_timer(struct kvm_vcpu *vcpu);
void __kvm_migrate_timers(struct kvm_vcpu *vcpu);

int apic_has_pending_timer(struct kvm_vcpu *vcpu);

int kvm_setup_default_irq_routing(struct kvm *kvm);
int kvm_setup_empty_irq_routing(struct kvm *kvm);
int kvm_irq_delivery_to_apic(struct kvm *kvm, struct kvm_lapic *src,
			     struct kvm_lapic_irq *irq,
			     struct dest_map *dest_map);

#endif
