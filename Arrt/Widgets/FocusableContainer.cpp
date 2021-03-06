#include <QApplication>
#include <QHBoxLayout>
#include <QPointer>
#include <QStylePainter>
#include <View/ArrtStyle.h>
#include <Widgets/FocusableContainer.h>

class FocusableContainerFocusListener : public QObject
{
public:
    typedef std::function<void(FocusableContainer* formControl, bool highlight)> Callback;

    FocusableContainerFocusListener(QApplication* application, Callback callback)
        : QObject(application)
        , m_callback(std::move(callback))
    {
        connect(application, &QApplication::focusChanged, this, [this](QWidget* /*old*/, QWidget* now) {
            FocusableContainer* newFocused = {};
            if (now)
            {
                auto* w = now;
                while (w != nullptr)
                {
                    if (newFocused = qobject_cast<FocusableContainer*>(w))
                    {
                        break;
                    }
                    w = w->parentWidget();
                }
            }
            if (newFocused != m_focusedFocusableContainer)
            {
                if (m_focusedFocusableContainer)
                {
                    m_callback(m_focusedFocusableContainer, false);
                }
                m_focusedFocusableContainer = newFocused;
                if (m_focusedFocusableContainer)
                {
                    m_callback(m_focusedFocusableContainer, true);
                }
            }
        });
    }

private:
    QPointer<FocusableContainer> m_focusedFocusableContainer;
    Callback m_callback;
};

FocusableContainer::FocusableContainer(QWidget* childWidget, QWidget* parent)
    : QWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    if (childWidget)
    {
        setChild(childWidget);
    }
}

void FocusableContainer::setChild(QWidget* child)
{
    assert(child);
    const int borderW = ArrtStyle::s_focusedControlBorderWidth;
    auto* l = new QHBoxLayout(this);
    l->setContentsMargins(borderW, borderW, -borderW, -borderW);
    l->addWidget(child);
}


QObject* FocusableContainer::installFocusListener(QApplication* application)
{
    auto onHighlighChanged = [](FocusableContainer* focusableContainer, bool highlight) {
        if (focusableContainer)
        {
            focusableContainer->setHighlight(highlight);
        }
    };

    return new FocusableContainerFocusListener(application, std::move(onHighlighChanged));
}

void FocusableContainer::paintEvent(QPaintEvent* e)
{
    if (m_highlighted)
    {
        QStylePainter p(this);
        ArrtStyle::drawFocusedBorder(&p, rect());
    }
    QWidget::paintEvent(e);
}

void FocusableContainer::setHighlight(bool highlight)
{
    if (m_highlighted != highlight)
    {
        m_highlighted = highlight;
        update();
    }
}
