1. 将signal()替换为sigaction(), sigaction获取信号的来源，如果信号来自内核则正常处理，否则丢弃
2. 使用setitimer替换alarm
