#include <QtDebug>
#include <QWheelEvent>

#include "mapview.h"

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent)
{
}

bool MapView::viewportEvent(QEvent *event)
{
    switch (event->type())
    {
        case 0:
            qDebug() << "QEvent::None"; break;  
        case 1:
            qDebug() << "QEvent::Timer"; break;  
        case 2:
            qDebug() << "QEvent::MouseButtonPress"; break;  
        case 3:
            qDebug() << "QEvent::MouseButtonRelease"; break;  
        case 4:
            qDebug() << "QEvent::MouseButtonDblClick"; break;  
        case 5:
            qDebug() << "QEvent::MouseMove"; break;  
        case 6:
            qDebug() << "QEvent::KeyPress"; break;  
        case 7:
            qDebug() << "QEvent::KeyRelease"; break;  
        case 8:
            qDebug() << "QEvent::FocusIn"; break;  
        case 9:
            qDebug() << "QEvent::FocusOut"; break;  
        case 10:
            qDebug() << "QEvent::Enter"; break; 
        case 11:
            qDebug() << "QEvent::Leave"; break; 
        case 12:
            qDebug() << "QEvent::Paint"; break; 
        case 13:
            qDebug() << "QEvent::Move"; break; 
        case 14:
            qDebug() << "QEvent::Resize"; break; 
        case 17:
            qDebug() << "QEvent::Show"; break; 
        case 18:
            qDebug() << "QEvent::Hide"; break; 
        case 19:
            qDebug() << "QEvent::Close"; break; 
        case 21:
            qDebug() << "QEvent::ParentChange"; break; 
        case 24:
            qDebug() << "QEvent::WindowActivate"; break; 
        case 25:
            qDebug() << "QEvent::WindowDeactivate"; break; 
        case 26:
            qDebug() << "QEvent::ShowToParent"; break; 
        case 27:
            qDebug() << "QEvent::HideToParent"; break; 
        case 31:
            qDebug() << "QEvent::Wheel"; break; 
        case 33:
            qDebug() << "QEvent::WindowTitleChange"; break; 
        case 34:
            qDebug() << "QEvent::WindowIconChange"; break; 
        case 35:
            qDebug() << "QEvent::ApplicationWindowIconChange"; break; 
        case 36:
            qDebug() << "QEvent::ApplicationFontChange"; break; 
        case 37:
            qDebug() << "QEvent::ApplicationLayoutDirectionChange"; break; 
        case 38:
            qDebug() << "QEvent::ApplicationPaletteChange"; break; 
        case 39:
            qDebug() << "QEvent::PaletteChange"; break; 
        case 40:
            qDebug() << "QEvent::Clipboard"; break; 
        case 43:
            qDebug() << "QEvent::MetaCall"; break; 
        case 50:
            qDebug() << "QEvent::SockAct"; break; 
        case 51:
            qDebug() << "QEvent::ShortcutOverride"; break; 
        case 52:
            qDebug() << "QEvent::DeferredDelete"; break; 
        case 60:
            qDebug() << "QEvent::DragEnter"; break; 
        case 61:
            qDebug() << "QEvent::DragMove"; break; 
        case 62:
            qDebug() << "QEvent::DragLeave"; break; 
        case 63:
            qDebug() << "QEvent::Drop"; break; 
        case 68:
            qDebug() << "QEvent::ChildAdded"; break; 
        case 69:
            qDebug() << "QEvent::ChildPolished"; break; 
        case 70:
            qDebug() << "QEvent::ChildInserted"; break; 
        case 71:
            qDebug() << "QEvent::ChildRemoved"; break; 
        case 74:
            qDebug() << "QEvent::PolishRequest"; break; 
        case 75:
            qDebug() << "QEvent::Polish"; break; 
        case 76:
            qDebug() << "QEvent::LayoutRequest"; break; 
        case 77:
            qDebug() << "QEvent::UpdateRequest"; break; 
        case 78:
            qDebug() << "QEvent::UpdateLater"; break; 
        case 82:
            qDebug() << "QEvent::ContextMenu"; break; 
        case 83:
            qDebug() << "QEvent::InputMethod"; break; 
        case 86:
            qDebug() << "QEvent::AccessibilityPrepare"; break; 
        case 87:
            qDebug() << "QEvent::TabletMove"; break; 
        case 88:
            qDebug() << "QEvent::LocaleChange"; break; 
        case 89:
            qDebug() << "QEvent::LanguageChange"; break; 
        case 90:
            qDebug() << "QEvent::LayoutDirectionChange"; break; 
        case 92:
            qDebug() << "QEvent::TabletPress"; break; 
        case 93:
            qDebug() << "QEvent::TabletRelease"; break; 
        case 94:
            qDebug() << "QEvent::OkRequest"; break; 
        case 96:
            qDebug() << "QEvent::IconDrag"; break; 
        case 97:
            qDebug() << "QEvent::FontChange"; break; 
        case 98:
            qDebug() << "QEvent::EnabledChange"; break; 
        case 99:
            qDebug() << "QEvent::ActivationChange"; break; 
        case 100:
            qDebug() << "QEvent::StyleChange"; break;
        case 101:
            qDebug() << "QEvent::IconTextChange"; break;
        case 102:
            qDebug() << "QEvent::ModifiedChange"; break;
        case 103:
            qDebug() << "QEvent::WindowBlocked"; break;
        case 104:
            qDebug() << "QEvent::WindowUnblocked"; break;
        case 105:
            qDebug() << "QEvent::WindowStateChange"; break;
        case 109:
            qDebug() << "QEvent::MouseTrackingChange"; break;
        case 110:
            qDebug() << "QEvent::ToolTip"; break;
        case 111:
            qDebug() << "QEvent::WhatsThis"; break;
        case 112:
            qDebug() << "QEvent::StatusTip"; break;
        case 113:
            qDebug() << "QEvent::ActionChanged"; break;
        case 114:
            qDebug() << "QEvent::ActionAdded"; break;
        case 115:
            qDebug() << "QEvent::ActionRemoved"; break;
        case 116:
            qDebug() << "QEvent::FileOpen"; break;
        case 117:
            qDebug() << "QEvent::Shortcut"; break;
        case 118:
            qDebug() << "QEvent::WhatsThisClicked"; break;
        case 119:
            qDebug() << "QEvent::AccessibilityHelp"; break;
        case 120:
            qDebug() << "QEvent::ToolBarChange"; break;
        case 121:
            qDebug() << "QEvent::ApplicationActivate"; break;
        case 122:
            qDebug() << "QEvent::ApplicationDeactivate"; break;
        case 123:
            qDebug() << "QEvent::QueryWhatsThis"; break;
        case 124:
            qDebug() << "QEvent::EnterWhatsThisMode"; break;
        case 125:
            qDebug() << "QEvent::LeaveWhatsThisMode"; break;
        case 126:
            qDebug() << "QEvent::ZOrderChange"; break;
        case 127:
            qDebug() << "QEvent::HoverEnter"; break;
        case 128:
            qDebug() << "QEvent::HoverLeave"; break;
        case 129:
            qDebug() << "QEvent::HoverMove"; break;
        case 130:
            qDebug() << "QEvent::AccessibilityDescription"; break;
        case 131:
            qDebug() << "QEvent::ParentAboutToChange"; break;
        case 132:
            qDebug() << "QEvent::WinEventAct"; break;
        case 150:
            qDebug() << "QEvent::EnterEditFocus"; break;
        case 151:
            qDebug() << "QEvent::LeaveEditFocus"; break;
        case 153:
            qDebug() << "QEvent::MenubarUpdated"; break;
        case 155:
            qDebug() << "QEvent::GraphicsSceneMouseMove"; break;
        case 156:
            qDebug() << "QEvent::GraphicsSceneMousePress"; break;
        case 157:
            qDebug() << "QEvent::GraphicsSceneMouseRelease"; break;
        case 158:
            qDebug() << "QEvent::GraphicsSceneMouseDoubleClick"; break;
        case 159:
            qDebug() << "QEvent::GraphicsSceneContextMenu"; break;
        case 160:
            qDebug() << "QEvent::GraphicsSceneHoverEnter"; break;
        case 161:
            qDebug() << "QEvent::GraphicsSceneHoverMove"; break;
        case 162:
            qDebug() << "QEvent::GraphicsSceneHoverLeave"; break;
        case 163:
            qDebug() << "QEvent::GraphicsSceneHelp"; break;
        case 164:
            qDebug() << "QEvent::GraphicsSceneDragEnter"; break;
        case 165:
            qDebug() << "QEvent::GraphicsSceneDragMove"; break;
        case 166:
            qDebug() << "QEvent::GraphicsSceneDragLeave"; break;
        case 167:
            qDebug() << "QEvent::GraphicsSceneDrop"; break;
        case 168:
            qDebug() << "QEvent::GraphicsSceneWheel"; break;
        case 169:
            qDebug() << "QEvent::KeyboardLayoutChange"; break;
        case 170:
            qDebug() << "QEvent::DynamicPropertyChange"; break;
        case 171:
            qDebug() << "QEvent::TabletEnterProximity"; break;
        case 172:
            qDebug() << "QEvent::TabletLeaveProximity"; break;
        case 173:
            qDebug() << "QEvent::NonClientAreaMouseMove"; break;
        case 174:
            qDebug() << "QEvent::NonClientAreaMouseButtonPress"; break;
        case 175:
            qDebug() << "QEvent::NonClientAreaMouseButtonRelease"; break;
        case 176:
            qDebug() << "QEvent::NonClientAreaMouseButtonDblClick"; break;
        case 177:
            qDebug() << "QEvent::MacSizeChange"; break;
        case 178:
            qDebug() << "QEvent::ContentsRectChange"; break;
        case 181:
            qDebug() << "QEvent::GraphicsSceneResize"; break;
        case 182:
            qDebug() << "QEvent::GraphicsSceneMove"; break;
        case 183:
            qDebug() << "QEvent::CursorChange"; break;
        case 184:
            qDebug() << "QEvent::ToolTipChange"; break;
        case 186:
            qDebug() << "QEvent::GrabMouse"; break;
        case 187:
            qDebug() << "QEvent::UngrabMouse"; break;
        case 188:
            qDebug() << "QEvent::GrabKeyboard"; break;
        case 189:
            qDebug() << "QEvent::UngrabKeyboard"; break;
        case 192:
            qDebug() << "QEvent::StateMachineSignal"; break;
        case 193:
            qDebug() << "QEvent::StateMachineWrapped"; break;
        case 194:
            qDebug() << "QEvent::TouchBegin"; break;
        case 195:
            qDebug() << "QEvent::TouchUpdate"; break;
        case 196:
            qDebug() << "QEvent::TouchEnd"; break;
        case 198:
            qDebug() << "QEvent::Gesture"; break;
        case 199:
            qDebug() << "QEvent::RequestSoftwareInputPanel"; break;
        case 200:
            qDebug() << "QEvent::CloseSoftwareInputPanel"; break;
        case 202:
            qDebug() << "QEvent::GestureOverride"; break;
        case 203:
            qDebug() << "QEvent::WinIdChange"; break;
    }

    return QGraphicsView::viewportEvent(event);
}
