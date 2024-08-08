local imgui = require 'imgui'
local key = require 'vkeys'

-- одно из основных отличий от оригинального апи
-- все переменные, значения которых записываются в ImGui по указателю, могут использоваться только через специальные типы
local main_window_state = imgui.ImBool(false)
function imgui.OnDrawFrame()
  if main_window_state.v then -- чтение и запись значения такой переменной осуществляется через поле v (или Value)
    imgui.SetNextWindowSize(imgui.ImVec2(150, 200), imgui.Cond.FirstUseEver) -- меняем размер
    -- но для передачи значения по указателю - обязательно напрямую
    -- тут main_window_state передаётся функции imgui.Begin, чтобы можно было отследить закрытие окна нажатием на крестик
    imgui.Begin('My window', main_window_state)
    imgui.Text('Hello world')
    if imgui.Button('Press me') then -- а вот и кнопка с действием
      -- условие будет выполнено при нажатии на неё
      printStringNow('Button pressed!', 1000)
    end
    imgui.End()
  end
end

function main()
  while true do
    wait(0)
    if wasKeyPressed(key.VK_X) then -- активация по нажатию клавиши X
        main_window_state.v = not main_window_state.v -- переключаем статус активности окна, не забываем про .v
    end
    imgui.Process = main_window_state.v -- теперь значение imgui.Process всегда будет задаваться в зависимости от активности основного окна
  end
end
