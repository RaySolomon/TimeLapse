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

#ifndef PIPELINE_RESIZE_FRAME_H
#define	PIPELINE_RESIZE_FRAME_H

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QTemporaryDir>

#include <Magick++.h>

#include "input_image_info.h"
#include "pipeline_handler.h"

namespace timelapse {
  
  class ResizeFrame : public ImageHandler {
    Q_OBJECT
  public:
    ResizeFrame(int w, int h);
  public slots:
    virtual void onInput(InputImageInfo info, Magick::Image img);

  private:
    int width;
    int height;
  };

}

#endif	/* PIPELINE_RESIZE_FRAME_H */

