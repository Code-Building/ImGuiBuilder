
void Gui_builder0()
{
ImGui::SetNextWindowSize({449.f,239.f});
ImGui::Begin("form id:1");
ImGui::SetCursorPos({170.f,109.f});
ImGui::Button("teste button", {0.f,0.f});


ImGui::End();
}
void Gui_builder1()
{
ImGui::SetNextWindowSize({375.f,179.f});
ImGui::Begin("form id:2");
ImGui::SetCursorPos(30.f,83.f);
ImGui::PushItemWidth(0.000000.f);
ImGui::InputText("txt_ryanGay","Text here",25);


ImGui::End();
}