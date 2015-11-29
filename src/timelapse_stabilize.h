/*
 *   Copyright (C) 2015 Lukáš Karas <lukas.karas@centrum.cz>
 *                                     
 *   This program is free software; you can redistribute it and/or modify  
 *   it under the terms of the GNU General Public License as published by  
 *   the Free Software Foundation; either version 2 of the License, or     
 *   (at your option) any later version.                                   
 *                                                                         
 *   This program is distributed in the hope that it will be useful,       
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
 *   GNU General Public License for more details.                          
 *                                                                         
 *   You should have received a copy of the GNU General Public License     
 *   along with this program; if not, write to the                         
 *   Free Software Foundation, Inc.,                                       
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             
 */

#ifndef TIMELAPSE_STABILIZE_H
#define	TIMELAPSE_STABILIZE_H


#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtCore/QTextStream>
#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QTemporaryDir>

#include <Magick++.h>

#include "vid.stab/libvidstab.h"

namespace timelapse {

  class TimeLapseStabilize : public QCoreApplication {
    Q_OBJECT

  public:
    TimeLapseStabilize(int &argc, char **argv);
    virtual ~TimeLapseStabilize();

  public slots:
    void run();
  signals:

  protected:
    QStringList parseArguments();

  protected:
    QTextStream _out;
    QTextStream _err;
    bool _dryRun;
  };
}

#endif	/* TIMELAPSE_STABILIZE_H */
