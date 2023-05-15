# done
- KVM是如何判断Triple Fault的
- 如果遇到需要注入多个事件怎么办
- kvm_queue_interrupt函数的作用
- 为什么有vmx_handle_exit_irqoff
- 外部中断被谁处理了，KVM有没有修改IDT指向新的handler
- 将对APIC-access page的访问映射到virtual APIC page后，那对于使用MSR访问的x2APIC怎么办？
- is_guest_mode
- KVM_IRQCHIP_NONE KVM_IRQCHIP_KERNEL KVM_IRQCHIP_SPLIT?
- guest执行中断程序时，中通过中断门跳转到内核态，发生了任务切换，这个时候需要跳转回host模拟任务切换？
- Intel 为什么不支持任务切换
- 
# 一些分析
- vmx_enable_nmi_window



# todo
- kvm_vcpu_pv_apf_data.flags
- schedule
- KVM如何处理进入guest后的可中断状态？会最终作用在进入guest后的EFLAG.IF上吗
- 既然外部中断被host处理了，那什么情况下才会传给guest，guest总不能接受不到外部中断吧
- KVM中虚拟化的IOAPIC是APIC还是xAPIC，还是x2APIC，虚拟化场景下还需要做区分吗？
- 当设备同时支持APIC，xAPIC，以及x2APIC时，对于虚拟设备来说，它会选择哪种来发起中断请求？
- 在已经虚拟化了PIC，IOAPIC，LAPIC的情况下，一个真实的外部中断如何传递到guest(KVM里面external interrupt exiting和NMI exiting已经被设置为min了,外部中断为host处理了)
- posted-interrupt 中Posted-interrupt notification vector只能表示一个vector，所以只能允许有一个中断vector被用于posted给guest，而除了这个之外其他还是得vm-exit？
- posted-interrupt 是可以独立存在的？（DMA remapping ，interrupt remapping）
