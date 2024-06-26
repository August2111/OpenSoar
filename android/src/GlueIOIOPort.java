// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

package de.opensoar;

import android.util.Log;
import ioio.lib.api.IOIO;
import ioio.lib.api.Uart;
import ioio.lib.api.exception.ConnectionLostException;

/**
 * ID = 0, pins TX=3, RX=4
 * ID = 1, pins TX=5, RX=6
 * ID = 2, pins TX=10 RX=11
 * ID = 3, pins TX=12 RX=13
 *
 */
final class GlueIOIOPort extends IOIOPort implements IOIOConnectionListener {
  private static final String TAG = "OpenSoar";

  private IOIOConnectionHolder holder;

  /**
   * Is the #IOIOConnectionHolder currently connected to an IOIO
   * board?
   */
  private boolean connected;

  private boolean constructing;

  private boolean failed;

  private final int inPin;
  private final int outPin;
  private int baudrate = 0;
  private int ID;

  GlueIOIOPort(IOIOConnectionHolder _holder, int ID_, int _baudrate) {
    super("IOIO UART " + ID_);

    ID = ID_;
    baudrate = _baudrate;

    switch (ID) {
    case 0:
    case 1:
      inPin = (ID * 2) + 4;
      outPin = inPin - 1;
      break;
    case 2:
    case 3:
      inPin = (ID * 2) + 7;
      outPin = inPin - 1;
      break;
    default:
      throw new IllegalArgumentException();
    }

    holder = _holder;
    _holder.addListener(this);
  }

  @Override public void onIOIOConnect(IOIO ioio)
    throws ConnectionLostException, InterruptedException {

    try {
      synchronized(this) {
        connected = true;
        constructing = true;
        failed = false;
      }

      stateChanged();

      Uart uart;
      try {
        uart = ioio.openUart(inPin, outPin, baudrate, Uart.Parity.NONE,
                             Uart.StopBits.ONE);
      } catch (Exception e) {
        submitError(e.getMessage());
        return;
      }

      set(uart);
    } finally {
      synchronized(this) {
        constructing = false;
        notifyAll();
      }

      stateChanged();
    }
  }

  @Override public void onIOIODisconnect(IOIO ioio) {
    connected = false;
    failed = true;
    stateChanged();

    super.close();
  }

  @Override public void close() {
    IOIOConnectionHolder holder;
    synchronized(this) {
      holder = this.holder;
      this.holder = null;
    }

    if (holder != null)
      holder.removeListener(this);
  }

  @Override public int getState() {
    synchronized(this) {
      if (failed)
        return STATE_FAILED;

      if (!connected || constructing)
        return STATE_LIMBO;
    }

    return super.getState();
  }

  @Override public int getBaudRate() {
    return baudrate;
  }

  @Override public boolean setBaudRate(int _baudrate) {
    if (_baudrate == baudrate)
      return true;

    IOIOConnectionHolder holder = this.holder;
    if (holder == null)
      /* this port was already closed */
      return false;

    final boolean wasConnected = connected;

    baudrate = _baudrate;
    holder.cycleListener(this);

    if (wasConnected) {
      try {
        /* wait until the port has been reconnected after a baud rate
           change; onIOIOConnect() will be called in another thread, and
           any attempt to do I/O before onIOIOConnect() has finished is
           doomed to fail */
        synchronized(this) {
          wait(200);
        }
      } catch (InterruptedException e) {
      }
    }

    return true;
  }
}
