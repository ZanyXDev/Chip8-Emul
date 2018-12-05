/*
 *  emul-qt - CHIP-8 emulator.
 *  Copyright (C) 2018  Pavel Mosentsev <zanyxdev@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "display.h"
#include <QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(chip8_res);
    QApplication app(argc, argv);

    app.setApplicationName("chip8-qt5");


    MainWindow mainWin;

    //    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&mw);
    //    mw.resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    //    mw.move((availableGeometry.width() - mw.width()) / 2,
    //            (availableGeometry.height() - mw.height()) / 2);

    mainWin.setWindowTitle(app.applicationName() + " " + app.applicationVersion());
    mainWin.show();


    return app.exec();
}
