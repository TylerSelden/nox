set confirm off

define dbg
  target remote localhost:1234

  break _start
  layout asm
  layout reg
  set print pretty on
end

define s
  si
end
