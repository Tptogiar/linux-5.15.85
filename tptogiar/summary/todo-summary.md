# done
- KVM是如何判断Triple Fault的
- 如果遇到需要注入多个事件怎么办
- kvm_queue_interrupt函数的作用
- 为什么有vmx_handle_exit_irqoff
- 外部中断被谁处理了，KVM有没有修改IDT指向新的handler
- 将对APIC-access page的访问映射到virtual APIC page后，那对于使用MSR访问的x2APIC怎么办？
- is_guest_mode

# 一些分析
- vmx_enable_nmi_window



# todo
- kvm_vcpu_pv_apf_data.flags
- schedule
- KVM如何处理进入guest后的可中断状态？会最终作用在进入guest后的EFLAG.IF上吗
- 既然外部中断被host处理了，那什么情况下才会传给guest，guest总不能接受不到外部中断吧
- Intel 为什么不支持任务切换
- guest执行中断程序时，中通过中断门跳转到内核态，发生了任务切换，这个时候需要跳转回host模拟任务切换？