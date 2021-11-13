#ifndef VIEWSPLIT_H
#define VIEWSPLIT_H

#include <QTabWidget>

#include <functional>

#include <buffer/buffer.h>
#include <core/global.h>

#include <QStyleOptionTab>
#include <QStylePainter>

class QToolButton;
class QMenu;
class QActionGroup;

namespace vnotex
{
    class ViewWindow;
    struct ViewWorkspace;

    class TabBar: public QTabBar
    {
    public:
        TabBar(QWidget *parent=0):QTabBar(parent){
        }

    protected:
        void paintEvent(QPaintEvent *){

            QStylePainter painter(this);
            QStyleOptionTab opt;
            QFont font=this->font();

            for(int i = 0;i < count();i++)
            {
                initStyleOption(&opt,i);
                painter.drawControl(QStyle::CE_TabBarTabShape, opt);
                if(QStyle::State_Selected & opt.state)
                {
                    painter.save();
                    font.setBold( true );
                    painter.setFont(font);
                    painter.drawControl(QStyle::CE_TabBarTabLabel,opt);
                    painter.restore();
                    continue;
                }
                else
                    painter.drawControl(QStyle::CE_TabBarTabLabel,opt);
           }
        }
    };

    class TabWidget : public QTabWidget
    {
    public:
        TabWidget(QWidget *parent=0):QTabWidget(parent){
            setTabBar(new TabBar(parent));
        }
    };

    //class ViewSplit : public QTabWidget
    class ViewSplit : public TabWidget
    {
        Q_OBJECT
    public:
        typedef std::function<bool(ViewWindow *)> ViewWindowSelector;

        struct ViewWindowNavigationModeInfo
        {
            // Top left position of the ViewWindow relative to the view split.
            QPoint m_topLeft;

            ViewWindow *m_viewWindow = nullptr;
        };

        ViewSplit(const QVector<QSharedPointer<ViewWorkspace>> &p_allWorkspaces,
                  const QSharedPointer<ViewWorkspace> &p_workspace,
                  ID p_id,
                  QWidget *p_parent = nullptr);

        ~ViewSplit();

        QVector<ViewWindow *> findBuffer(const Buffer *p_buffer) const;

        int getViewWindowCount() const;

        void addViewWindow(ViewWindow *p_win);

        ViewWindow *getCurrentViewWindow() const;
        void setCurrentViewWindow(ViewWindow *p_win);

        void setCurrentViewWindow(int p_idx);

        // @p_win is not deleted.
        void takeViewWindow(ViewWindow *p_win);

        void setWorkspace(const QSharedPointer<ViewWorkspace> &p_workspace);

        QSharedPointer<ViewWorkspace> getWorkspace() const;

        void setActive(bool p_active);

        // @p_func: return true if going well, return false to stop the iteration.
        // Return false if there is a break.
        bool forEachViewWindow(const ViewWindowSelector &p_func) const;

        QVector<ViewWindowNavigationModeInfo> getNavigationModeInfo() const;

        void focus();

        ID getId() const;

        void updateStateToWorkspace() const;

    signals:
        void viewWindowCloseRequested(ViewWindow *p_win);

        void verticalSplitRequested(ViewSplit *p_split);

        void horizontalSplitRequested(ViewSplit *p_split);

        void maximizeSplitRequested(ViewSplit *p_split);

        void distributeSplitsRequested();

        void removeSplitRequested(ViewSplit *p_split);

        void removeSplitAndWorkspaceRequested(ViewSplit *p_split);

        void newWorkspaceRequested(ViewSplit *p_split);

        void removeWorkspaceRequested(ViewSplit *p_split);

        void focused(ViewSplit *p_split);

        void currentViewWindowChanged(ViewWindow *p_win);

        void moveViewWindowOneSplitRequested(ViewSplit *p_split, ViewWindow *p_win, Direction p_direction);

    protected:
        bool eventFilter(QObject *p_object, QEvent *p_event) Q_DECL_OVERRIDE;

        void mousePressEvent(QMouseEvent *p_event) Q_DECL_OVERRIDE;

        // To accept specific drop.
        void dragEnterEvent(QDragEnterEvent *p_event) Q_DECL_OVERRIDE;

        // Drop the data.
        void dropEvent(QDropEvent *p_event) Q_DECL_OVERRIDE;

    private slots:
        void closeTab(int p_idx);

    private:
        void setupUI();

        void setupCornerWidget();

        void setupTabBar();

        void setupShortcuts();

        ViewWindow *getViewWindow(int p_idx) const;

        void updateAndTakeCurrentWorkspace();

        void initIcons();

        void updateWindowList(QMenu *p_menu);

        void updateMenu(QMenu *p_menu);

        void createContextMenuOnTabBar(QMenu *p_menu, int p_tabIdx);

        void focusCurrentViewWindow();

        void alternateTab();

        void activateNextTab(bool p_backward);

        ID m_id = 0;

        const QVector<QSharedPointer<ViewWorkspace>> &m_allWorkspaces;

        QSharedPointer<ViewWorkspace> m_workspace;

        QToolButton *m_windowListButton = nullptr;

        QToolButton *m_menuButton = nullptr;

        QActionGroup *m_windowListActionGroup = nullptr;

        QActionGroup *m_workspaceActionGroup = nullptr;

        // Used for AlternateTab.
        ViewWindow *m_currentViewWindow = nullptr;
        ViewWindow *m_lastViewWindow = nullptr;

        static QIcon s_windowListIcon;

        static QIcon s_windowListActiveIcon;

        static QIcon s_menuIcon;

        static QIcon s_menuActiveIcon;

        static const QString c_activeActionButtonForegroundName;

        static const QString c_actionButtonForegroundName;
    };
} // ns vnotex

#endif // VIEWSPLIT_H
