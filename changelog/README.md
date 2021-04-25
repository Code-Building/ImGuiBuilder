# CHANGELOG

----------------------------------------------------------------------------------------------------------------------------------------------
- 24/04/2021 
## - Problems solved.
    - Problem with deleting the selected control even with the builder window in the background.
    - - Now all checks of key presses have a check if the window in the foreground is from ImGuiBuilder.
    - A bug that could happen to save just by clicking on the option and not on the confirmation of the "OK" button.
    - The code generation had a bug in the button code generation, where a "{" key was missing.
    - Resolved the bug of copying selected control when one of the editor windows is active.
    - Load config was leaving controls for the project created earlier in the new one.
    - When adding a child without having a form, the project crashed.
    - Removed centering the control to the position of the cursor at the time of moving.

## - Improvements.
    - The move_item has been improved, the code has been optimized and organized, the frame count has been removed for the "hold the click to move" exchanged for verification in ms.
    - Improved the definition of object selection by placing a border.
    - Improved the system of moving only if the mouse click is held.
    - Movement of forms is now only by title, avoid moving the form when trying to move a control.
    - Move child only at the border of the embroidery.
    - The Code was organized

## - Added.
    - Added to save and open settings, ImGuiFileDialog
    - Added the option to resize the controls.
    - Added a context menu when right clicking on a control, having the options to delete, copy and others. 