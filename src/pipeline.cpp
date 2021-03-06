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

#include <exception>

#include "pipeline.h"
#include "pipeline.moc"
#include "pipeline_handler.h"
#include "pipeline_cpt.h"

namespace timelapse {

  Pipeline* Pipeline::createWithCaptureSource(QSharedPointer<CaptureDevice> dev, int64_t interval, int32_t cnt,
    QTextStream *verboseOutput, QTextStream *err) {

    PipelineCaptureSource *src = new PipelineCaptureSource(dev, interval, cnt, verboseOutput, err);
    return new Pipeline(src, src, verboseOutput, err);
  }

  Pipeline* Pipeline::createWithFileSource(QStringList inputArguments, bool recursive,
    QTextStream *verboseOutput, QTextStream *err) {

    PipelineFileSource *src = new PipelineFileSource(inputArguments, recursive, verboseOutput, err);
    return new Pipeline(src, src, verboseOutput, err);
  }

  Pipeline::Pipeline(PipelineSource *src, InputHandler *firstInputHandler,
    QTextStream *verboseOutput, QTextStream *err) :
  verboseOutput(verboseOutput), err(err),
  elements(), src(src), lastInputHandler(firstInputHandler), lastImageHandler(NULL) {

    append(firstInputHandler);
  }

  Pipeline::Pipeline(PipelineSource *src, ImageHandler *firstImageHandler,
    QTextStream *verboseOutput, QTextStream *err) :
  verboseOutput(verboseOutput), err(err),
  elements(), src(src), lastInputHandler(NULL), lastImageHandler(firstImageHandler) {

    append(firstImageHandler);
  }

  Pipeline::~Pipeline() {
    for (QObject *el : elements) {
      delete el;
    }
  }

  void Pipeline::handlerFinished() {
    QObject *sender = QObject::sender();
    if (sender != NULL)
      *verboseOutput << "Pipeline handler " << sender->metaObject()->className() << " finished" << endl;
    else
      *verboseOutput << "handlerFinished called directly!" << endl;
  }

  void Pipeline::onError(QString msg) {
    QObject *sender = QObject::sender();
    if (sender != NULL) {
      *verboseOutput << "Error in pipeline handler " << sender->metaObject()->className() << " :" << endl;
    } else {
      *verboseOutput << "onError slot called directly!" << endl;
    }
    *err << msg << endl;
  }

  void Pipeline::append(PipelineHandler *handler) {
    *verboseOutput << "Pipeline append " << handler->metaObject()->className() << "" << endl;
    connect(handler, SIGNAL(last()), this, SLOT(handlerFinished()));
    connect(handler, SIGNAL(error(QString)), this, SLOT(onError(QString)));
    connect(handler, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    elements.append(handler);
  }

  void Pipeline::operator<<(ImageHandler *handler) {

    if (lastInputHandler != NULL) {
      ImageLoader *loader = new ImageLoader(verboseOutput, err);
      connect(lastInputHandler, SIGNAL(input(InputImageInfo)), loader, SLOT(onInput(InputImageInfo)));
      connect(lastInputHandler, SIGNAL(last()), loader, SLOT(onLast()));
      append(loader);

      lastInputHandler = NULL;
      lastImageHandler = loader;
    }

    if (lastImageHandler != NULL) {

      connect(lastImageHandler, SIGNAL(input(InputImageInfo, Magick::Image)), handler, SLOT(onInput(InputImageInfo, Magick::Image)));
      connect(lastImageHandler, SIGNAL(last()), handler, SLOT(onLast()));
      //*verboseOutput << "Connect " << lastImageHandler->metaObject()->className()
      //  << " to " << handler->metaObject()->className() << endl;

    } else {
      throw runtime_error("Weird pipeline state");
    }
    lastInputHandler = NULL;
    lastImageHandler = handler;

    append(handler);
  }

  void Pipeline::operator<<(InputHandler *handler) {
    if (lastImageHandler != NULL) {
      ImageTrash *trash = new ImageTrash();
      connect(lastImageHandler, SIGNAL(input(InputImageInfo, Magick::Image)), trash, SLOT(onInput(InputImageInfo, Magick::Image)));
      connect(lastImageHandler, SIGNAL(last()), trash, SLOT(onLast()));
      append(trash);

      lastImageHandler = NULL;
      lastInputHandler = trash;
    }

    if (lastInputHandler != NULL) {
      connect(lastInputHandler, SIGNAL(input(InputImageInfo)), handler, SLOT(onInput(InputImageInfo)));
      connect(lastInputHandler, SIGNAL(last()), handler, SLOT(onLast()));

    } else {
      throw runtime_error("Weird pipeline state");
    }
    lastInputHandler = handler;
    lastImageHandler = NULL;

    append(handler);
  }

  void Pipeline::process() {
    // listen when last element finish their job
    if (lastInputHandler != NULL) {
      connect(lastInputHandler, SIGNAL(last()), this, SIGNAL(done()));
    } else if (lastImageHandler != NULL) {
      connect(lastImageHandler, SIGNAL(last()), this, SIGNAL(done()));
    } else {
      throw logic_error("No handler in pipeline");
      //connect(src, SIGNAL(last()), this, SIGNAL(done()));
    }

    emit src->process();
  }
}
