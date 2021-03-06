/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <omni/ui/InputPreview.h>

#include <QDebug>
#include <QResizeEvent>
#include <QMatrix4x4>
#include <omni/visual/util.h>
#include <omni/visual/Rectangle.h>

namespace omni
{
  namespace ui
  {
    InputPreview::InputPreview(QWidget* _parent) :
      GLView(_parent)
    {
      setMinimumSize(128 / devicePixelRatio(),128 / devicePixelRatio());
    }

    InputPreview::InputPreview(input::Interface* _input, QWidget* _parent) :
        GLView(_parent),
        input_(_input) {
    }

    InputPreview::~InputPreview()
    {
    }

    float InputPreview::border() const
    {
      return border_;
    }

    void InputPreview::setBorder(float _border)
    {
      border_ = _border;
      update();
    }

    void InputPreview::mouseMoveEvent(QMouseEvent *event)
    {
      if (!input()) return;
      if (event->buttons() & Qt::LeftButton)
      {
        setRulerPos(event->pos());
      }
    }

    void InputPreview::mousePressEvent(QMouseEvent *event)
    {
      GLView::mousePressEvent(event);
      if (!input()) return;

      setRulerPos(event->pos());
    }

    void InputPreview::mouseReleaseEvent(QMouseEvent *event)
    {
      if (!input()) return;
      if (event->buttons() & Qt::LeftButton)
      {
        setRulerPos(event->pos());
      }
    }

    void InputPreview::setRulerPos(QPointF const& _eventPos)
    {
      if (!input()) return;

      QPointF _pos = screenPos(_eventPos);
      input()->setRulerPos(_pos);
      emit inputChanged();
      update();
    }

    input::Interface* InputPreview::input()
    {
      return input_;
    }

    input::Interface const* InputPreview::input() const
    {
      return input_;
    }

     void InputPreview::setInput(input::Interface* _input) {
         input_=_input;
     }

    bool InputPreview::initialize()
    {
      return context() != nullptr;
    }

    QRectF InputPreview::viewRect() const
    {
      auto* _input = input();
      if (!_input) return QRectF(0.0,0.0,1.0,1.0);

      float _projAspect = float(_input->width()) /
                          _input->height();
      float _viewAspect = float(width()) / height();
      float b = border_ * 0.5;
      float _left = -0.5 - b,_right = 0.5 + b,_bottom = -0.5 - b,_top = 0.5 + b;

      if (_projAspect > _viewAspect)
      {
        _top *= _projAspect / _viewAspect;
        _bottom *=  _projAspect / _viewAspect;
      }
      else
      {
        _left *= _viewAspect / _projAspect;
        _right *= _viewAspect / _projAspect;
      }

      return QRectF(QPointF(_left,_top),QPointF(_right,_bottom));
    }

    QPointF InputPreview::screenPos(QPointF const& _pos) const
    {
      QRectF&& _rect = viewRect();
      QPointF _p = QPointF(_pos.x() / width(),_pos.y() / height() );
      return QPointF(_p.x() * _rect.width(),-_p.y() * _rect.height());
    }

    void InputPreview::paintGL()
    {
      visual::with_current_context([this](QOpenGLFunctions& _)
      {
        _.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _.glDisable(GL_BLEND);
      });

      if (!input_) return;

      auto _rect = viewRect();

      input_->update();
      makeCurrent();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      visual::viewport(this);

      /// Setup orthogonal projection
      glMatrixMode(GL_PROJECTION);
      {
        glLoadIdentity();
        QMatrix4x4 _m;
        _m.ortho(_rect.left(),_rect.right(),_rect.top(),_rect.bottom(),-1.0,1.0);
        glMultMatrixf(_m.constData());
      }

      visual::with_current_context([&](QOpenGLFunctions& _)
      {
        _.glEnable(GL_TEXTURE_2D);
        _.glBindTexture(GL_TEXTURE_2D, input_->textureId());
        visual::Rectangle::draw();
        _.glBindTexture(GL_TEXTURE_2D, 0);
      });
    }
  }
}
