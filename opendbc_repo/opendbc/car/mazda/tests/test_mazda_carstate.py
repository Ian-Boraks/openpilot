#!/usr/bin/env python3
"""
Tests for Mazda CarState, specifically the AOL (Always On Lateral) persistence feature.

The AOL persistence feature tracks when cruise control has been engaged at least once,
allowing lateral control to stay active after cruise was set once (until cruise is turned off).
"""
import pytest
from types import SimpleNamespace
from unittest.mock import MagicMock, patch

from opendbc.can import CANPacker, CANParser
from opendbc.car import Bus
from opendbc.car.mazda.carstate import CarState
from opendbc.car.mazda.values import CAR, DBC


class MockFrogPilotToggles:
  """Mock frogpilot_toggles object for testing"""
  def __init__(self, always_on_lateral=False):
    self.always_on_lateral = always_on_lateral


class MockCANParsers:
  """Creates mock CAN parsers with configurable values for Mazda"""

  def __init__(self, car_fingerprint=CAR.MAZDA_CX5):
    self.dbc = DBC[car_fingerprint][Bus.pt]
    self.packer = CANPacker(self.dbc)

    # Default values matching Mazda DBC structure
    self.pt_values = {
      "WHEEL_SPEEDS": {"FL": 0, "FR": 0, "RL": 0, "RR": 0},
      "ENGINE_DATA": {"SPEED": 0, "PEDAL_GAS": 0},
      "GEAR": {"GEAR": 0},
      "BLINK_INFO": {"HIGH_BEAMS": 0, "LEFT_BLINK": 0, "RIGHT_BLINK": 0},
      "BSM": {"LEFT_BS_STATUS": 0, "RIGHT_BS_STATUS": 0},
      "STEER": {"STEER_ANGLE": 0},
      "STEER_TORQUE": {"STEER_TORQUE_SENSOR": 0, "STEER_TORQUE_MOTOR": 0},
      "STEER_RATE": {"STEER_ANGLE_RATE": 0, "LKAS_BLOCK": 0, "HANDS_OFF_5_SECONDS": 0},
      "PEDALS": {"BRAKE_ON": 0, "STANDSTILL": 0},
      "BRAKE": {"BRAKE_PRESSURE": 0},
      "SEATBELT": {"DRIVER_SEATBELT": 1},
      "DOORS": {"FL": 0, "FR": 0, "BL": 0, "BR": 0},
      "CRZ_CTRL": {"CRZ_AVAILABLE": 0, "CRZ_ACTIVE": 0},
      "CRZ_EVENTS": {"CRZ_SPEED": 0},
      "CRZ_BTNS": {"CTR": 0, "DISTANCE_LESS": 0},
    }

    self.cam_values = {
      "CAM_LANEINFO": {"LANE_LINES": 1},
      "CAM_LKAS": {"ERR_BIT_1": 0},
    }

  def create_parsers(self):
    """Create mock CAN parsers that return our configured values"""
    pt_parser = MagicMock()
    cam_parser = MagicMock()

    # Setup pt_parser.vl to return our values
    pt_parser.vl = self.pt_values
    cam_parser.vl = self.cam_values

    return {
      Bus.pt: pt_parser,
      Bus.cam: cam_parser,
    }

  def set_cruise_available(self, available: bool):
    """Set cruise control available (main switch on)"""
    self.pt_values["CRZ_CTRL"]["CRZ_AVAILABLE"] = 1 if available else 0

  def set_cruise_enabled(self, enabled: bool):
    """Set cruise control enabled (cruise active)"""
    self.pt_values["CRZ_CTRL"]["CRZ_ACTIVE"] = 1 if enabled else 0

  def set_speed_kph(self, speed: float):
    """Set vehicle speed in KPH"""
    self.pt_values["ENGINE_DATA"]["SPEED"] = speed


class TestMazdaCarStateAOL:
  """Tests for Always On Lateral persistence in Mazda CarState"""

  @pytest.fixture
  def mock_cp(self):
    """Create a mock CarParams object with required numerical values"""
    cp = MagicMock()
    cp.carFingerprint = CAR.MAZDA_CX5
    cp.minSteerSpeed = 0
    # These need to be real numbers, not MagicMocks
    cp.wheelSpeedFactor = 1.0
    cp.vEgoStopping = 0.1
    cp.vEgoStarting = 0.1
    cp.steerLimitAlert = False
    return cp

  @pytest.fixture
  def mock_fpcp(self):
    """Create a mock FrogPilot CarParams object"""
    return MagicMock()

  @pytest.fixture
  def car_state(self, mock_cp, mock_fpcp):
    """Create a CarState instance for testing"""
    # Patch CANDefine to avoid needing actual DBC parsing
    with patch('opendbc.car.mazda.carstate.CANDefine') as mock_can_define:
      mock_can_define.return_value.dv = {"GEAR": {"GEAR": {}}}
      cs = CarState(mock_cp, mock_fpcp)
      # Initialize low_speed_alert since it's used in update()
      cs.low_speed_alert = False
      return cs

  @pytest.fixture
  def can_parsers(self):
    """Create mock CAN parsers"""
    return MockCANParsers()

  def test_cruise_previously_engaged_initial_state(self, car_state):
    """Test that cruise_previously_engaged starts as False"""
    assert car_state.cruise_previously_engaged is False

  def test_cruise_previously_engaged_set_on_enable(self, car_state, can_parsers):
    """Test that cruise_previously_engaged is set to True when cruise is enabled"""
    toggles = MockFrogPilotToggles(always_on_lateral=True)

    # Initial state - cruise available but not enabled
    can_parsers.set_cruise_available(True)
    can_parsers.set_cruise_enabled(False)

    ret, fp_ret = car_state.update(can_parsers.create_parsers(), toggles)
    assert car_state.cruise_previously_engaged is False

    # Enable cruise
    can_parsers.set_cruise_enabled(True)
    ret, fp_ret = car_state.update(can_parsers.create_parsers(), toggles)
    assert car_state.cruise_previously_engaged is True

  def test_cruise_previously_engaged_persists_after_disable(self, car_state, can_parsers):
    """Test that cruise_previously_engaged stays True after cruise is disabled but main switch is still on"""
    toggles = MockFrogPilotToggles(always_on_lateral=True)

    # Enable cruise
    can_parsers.set_cruise_available(True)
    can_parsers.set_cruise_enabled(True)
    car_state.update(can_parsers.create_parsers(), toggles)
    assert car_state.cruise_previously_engaged is True

    # Disable cruise (cancel) but keep main switch on
    can_parsers.set_cruise_enabled(False)
    ret, fp_ret = car_state.update(can_parsers.create_parsers(), toggles)

    # Should still be True because cruise is still available
    assert car_state.cruise_previously_engaged is True

  def test_cruise_previously_engaged_resets_on_main_off(self, car_state, can_parsers):
    """Test that cruise_previously_engaged resets when cruise main switch is turned off"""
    toggles = MockFrogPilotToggles(always_on_lateral=True)

    # Enable cruise
    can_parsers.set_cruise_available(True)
    can_parsers.set_cruise_enabled(True)
    car_state.update(can_parsers.create_parsers(), toggles)
    assert car_state.cruise_previously_engaged is True

    # Turn off main switch
    can_parsers.set_cruise_available(False)
    can_parsers.set_cruise_enabled(False)
    ret, fp_ret = car_state.update(can_parsers.create_parsers(), toggles)

    # Should reset to False
    assert car_state.cruise_previously_engaged is False

  def test_aol_allowed_requires_both_conditions(self, car_state, can_parsers):
    """Test that alwaysOnLateralAllowed requires both cruise_previously_engaged AND cruise available"""
    toggles = MockFrogPilotToggles(always_on_lateral=True)

    # Case 1: Cruise never engaged, cruise available
    can_parsers.set_cruise_available(True)
    can_parsers.set_cruise_enabled(False)
    ret, fp_ret = car_state.update(can_parsers.create_parsers(), toggles)
    assert fp_ret.alwaysOnLateralAllowed is False

    # Case 2: Engage cruise (both conditions met)
    can_parsers.set_cruise_enabled(True)
    ret, fp_ret = car_state.update(can_parsers.create_parsers(), toggles)
    assert fp_ret.alwaysOnLateralAllowed is True

    # Case 3: Disable cruise but keep main on (should still be allowed - AOL persistence!)
    can_parsers.set_cruise_enabled(False)
    ret, fp_ret = car_state.update(can_parsers.create_parsers(), toggles)
    assert fp_ret.alwaysOnLateralAllowed is True

    # Case 4: Turn off main switch (should no longer be allowed)
    can_parsers.set_cruise_available(False)
    ret, fp_ret = car_state.update(can_parsers.create_parsers(), toggles)
    assert fp_ret.alwaysOnLateralAllowed is False

  def test_aol_allowed_only_when_toggle_enabled(self, car_state, can_parsers):
    """Test that alwaysOnLateralAllowed is only set when always_on_lateral toggle is enabled"""
    # With AOL toggle disabled
    toggles_disabled = MockFrogPilotToggles(always_on_lateral=False)

    can_parsers.set_cruise_available(True)
    can_parsers.set_cruise_enabled(True)
    ret, fp_ret = car_state.update(can_parsers.create_parsers(), toggles_disabled)

    # Should be False (default) because toggle is disabled
    assert fp_ret.alwaysOnLateralAllowed is False

    # With AOL toggle enabled
    toggles_enabled = MockFrogPilotToggles(always_on_lateral=True)
    ret, fp_ret = car_state.update(can_parsers.create_parsers(), toggles_enabled)

    # Now should be True
    assert fp_ret.alwaysOnLateralAllowed is True

  def test_full_aol_persistence_cycle(self, car_state, can_parsers):
    """Test a full cycle of AOL persistence: enable cruise, cancel, re-enable without needing to SET again"""
    toggles = MockFrogPilotToggles(always_on_lateral=True)

    # Step 1: Turn on cruise main switch
    can_parsers.set_cruise_available(True)
    can_parsers.set_cruise_enabled(False)
    ret, fp_ret = car_state.update(can_parsers.create_parsers(), toggles)
    assert fp_ret.alwaysOnLateralAllowed is False
    assert car_state.cruise_previously_engaged is False

    # Step 2: Set cruise (engage)
    can_parsers.set_cruise_enabled(True)
    ret, fp_ret = car_state.update(can_parsers.create_parsers(), toggles)
    assert fp_ret.alwaysOnLateralAllowed is True
    assert car_state.cruise_previously_engaged is True

    # Step 3: Cancel cruise (but keep main on) - AOL should persist
    can_parsers.set_cruise_enabled(False)
    ret, fp_ret = car_state.update(can_parsers.create_parsers(), toggles)
    assert fp_ret.alwaysOnLateralAllowed is True  # <-- This is the key AOL persistence behavior
    assert car_state.cruise_previously_engaged is True

    # Step 4: Re-engage cruise - should work seamlessly
    can_parsers.set_cruise_enabled(True)
    ret, fp_ret = car_state.update(can_parsers.create_parsers(), toggles)
    assert fp_ret.alwaysOnLateralAllowed is True
    assert car_state.cruise_previously_engaged is True

    # Step 5: Turn off main switch - AOL should stop
    can_parsers.set_cruise_available(False)
    can_parsers.set_cruise_enabled(False)
    ret, fp_ret = car_state.update(can_parsers.create_parsers(), toggles)
    assert fp_ret.alwaysOnLateralAllowed is False
    assert car_state.cruise_previously_engaged is False


if __name__ == "__main__":
  pytest.main([__file__, "-v"])
