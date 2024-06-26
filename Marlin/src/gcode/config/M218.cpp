/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if HAS_HOTEND_OFFSET

#include "../gcode.h"
#include "../../module/motion.h"

#if ENABLED(DELTA)
  #include "../../module/planner.h"
#endif

#if (MOTHERBOARD == BOARD_SNAPMAKER_2_0)
  #include "../../../../snapmaker/src/module/toolhead_dualextruder.h"
#endif

/**
 * M218 - set hotend offset (in linear units)
 *
 *   T<tool>
 *   X<xoffset>
 *   Y<yoffset>
 *   Z<zoffset>
 */
void GcodeSuite::M218() {

  const int8_t target_extruder = get_target_extruder_from_command();
  if (target_extruder < 0) return;

  if (parser.seenval('X')) {
    hotend_offset[X_AXIS][target_extruder] = parser.value_linear_units();
    if (target_extruder)
      printer1->ModuleCtrlSaveHotendOffset(hotend_offset[X_AXIS][1], X_AXIS);
  }
  if (parser.seenval('Y')) {
    hotend_offset[Y_AXIS][target_extruder] = parser.value_linear_units();
    if (target_extruder)
      printer1->ModuleCtrlSaveHotendOffset(hotend_offset[Y_AXIS][1], Y_AXIS);
  }
  if (parser.seenval('Z')) {
    hotend_offset[Z_AXIS][target_extruder] = parser.value_linear_units();
    if (target_extruder)
      printer1->ModuleCtrlSaveHotendOffset(hotend_offset[Z_AXIS][1], Z_AXIS);
  }

  if (!parser.seen("XYZ")) {
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM(MSG_HOTEND_OFFSET);
    HOTEND_LOOP() {
      SERIAL_CHAR(' ');
      SERIAL_ECHO(hotend_offset[X_AXIS][e]);
      SERIAL_CHAR(',');
      SERIAL_ECHO(hotend_offset[Y_AXIS][e]);
      SERIAL_CHAR(',');
      SERIAL_ECHO_F(hotend_offset[Z_AXIS][e], 3);
    }
    SERIAL_EOL();
  }

  #if ENABLED(DELTA)
    if (target_extruder == active_extruder)
      do_blocking_move_to_xy(current_position[X_AXIS], current_position[Y_AXIS], planner.settings.max_feedrate_mm_s[X_AXIS]);
  #endif
}

#endif // HAS_HOTEND_OFFSET
