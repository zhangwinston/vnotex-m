#include "newnotedialog.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QPushButton>
#include <QPlainTextEdit>

#include "notebook/notebook.h"
#include "notebook/node.h"
#include "../widgetsfactory.h"
#include <utils/pathutils.h>
#include <utils/fileutils.h>
#include "exception.h"
#include "nodeinfowidget.h"
#include <utils/widgetutils.h>
#include <core/templatemgr.h>
#include <core/configmgr.h>
#include <core/widgetconfig.h>
#include <snippet/snippetmgr.h>
#include <buffer/filetypehelper.h>

using namespace vnotex;

QString NewNoteDialog::s_lastTemplate;

NewNoteDialog::NewNoteDialog(Node *p_node, QWidget *p_parent)
    : ScrollDialog(p_parent)
{
    Q_ASSERT(p_node && p_node->isLoaded());
    setupUI(p_node);

    initDefaultValues(p_node);

    m_infoWidget->getNameLineEdit()->setFocus();
}

void NewNoteDialog::setupUI(const Node *p_node)
{
    setupNodeInfoWidget(p_node, this);
    setCentralWidget(m_infoWidget);

    auto infoLayout = m_infoWidget->getMainLayout();

    {
        auto templateLayout = new QHBoxLayout();
        templateLayout->setContentsMargins(0, 0, 0, 0);
        infoLayout->addRow(tr("Template:"), templateLayout);

        setupTemplateComboBox(m_infoWidget);
        templateLayout->addWidget(m_templateComboBox);

        templateLayout->addStretch();

        auto manageBtn = new QPushButton(tr("Manage"), m_infoWidget);
        templateLayout->addWidget(manageBtn);
        connect(manageBtn, &QPushButton::clicked,
                this, []() {
                    WidgetUtils::openUrlByDesktop(QUrl::fromLocalFile(TemplateMgr::getInst().getTemplateFolder()));
                });

        m_templateTextEdit = WidgetsFactory::createPlainTextConsole(m_infoWidget);
        infoLayout->addRow("", m_templateTextEdit);
        m_templateTextEdit->hide();
    }

    setDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    setWindowTitle(tr("New Note"));
}

void NewNoteDialog::setupNodeInfoWidget(const Node *p_node, QWidget *p_parent)
{
    m_infoWidget = new NodeInfoWidget(p_node, Node::Flag::Content, p_parent);
}

bool NewNoteDialog::validateInputs()
{
    bool valid = true;
    QString msg;

    valid = valid && validateNameInput(msg);
    setInformationText(msg, valid ? ScrollDialog::InformationLevel::Info
                                  : ScrollDialog::InformationLevel::Error);
    return valid;
}

bool NewNoteDialog::validateNameInput(QString &p_msg)
{
    p_msg.clear();

    auto name = m_infoWidget->getName();
    if (name.isEmpty() || !PathUtils::isLegalFileName(name)) {
        p_msg = tr("Please specify a valid name for the note.");
        return false;
    }

    if (!m_infoWidget->getParentNode()->isLegalNameForNewChild(name)) {
        p_msg = tr("Name conflicts with existing or built-in note.");
        return false;
    }

    return true;
}

void NewNoteDialog::acceptedButtonClicked()
{
    s_lastTemplate = m_templateComboBox->currentData().toString();

    {
        auto fileType = FileTypeHelper::getInst().getFileTypeByName(m_infoWidget->getFileType()).m_type;
        ConfigMgr::getInst().getWidgetConfig().setNewNoteDefaultFileType(static_cast<int>(fileType));
    }

    if (validateInputs() && newNote()) {
        accept();
    }
}

bool NewNoteDialog::newNote()
{
    m_newNode.clear();

    Notebook *notebook = const_cast<Notebook *>(m_infoWidget->getNotebook());
    Node *parentNode = const_cast<Node *>(m_infoWidget->getParentNode());
    try {
        m_newNode = notebook->newNode(parentNode,
                                      Node::Flag::Content,
                                      m_infoWidget->getName(),
                                      getTemplateContent());
    } catch (Exception &p_e) {
        QString msg = tr("Failed to create note under (%1) in (%2) (%3).").arg(parentNode->getName(),
                                                                               notebook->getName(),
                                                                               p_e.what());
        qCritical() << msg;
        setInformationText(msg, ScrollDialog::InformationLevel::Error);
        return false;
    }

    emit notebook->nodeUpdated(m_newNode.data());

    return true;
}

const QSharedPointer<Node> &NewNoteDialog::getNewNode() const
{
    return m_newNode;
}

void NewNoteDialog::initDefaultValues(const Node *p_node)
{
    {
        int defaultType = ConfigMgr::getInst().getWidgetConfig().getNewNoteDefaultFileType();
        const auto &fileType = FileTypeHelper::getInst().getFileType(defaultType);

        m_infoWidget->setFileType(fileType.m_typeName);

        auto lineEdit = m_infoWidget->getNameLineEdit();
        auto defaultName = FileUtils::generateFileNameWithSequence(p_node->fetchAbsolutePath(),
                                                                   tr("note"),
                                                                   fileType.preferredSuffix());
        lineEdit->setText(defaultName);
        WidgetUtils::selectBaseName(lineEdit);
    }

    if (!s_lastTemplate.isEmpty()) {
        // Restore.
        int idx = m_templateComboBox->findData(s_lastTemplate);
        if (idx != -1) {
            m_templateComboBox->setCurrentIndex(idx);
        } else {
            s_lastTemplate.clear();
        }
    }
}

void NewNoteDialog::setupTemplateComboBox(QWidget *p_parent)
{
    m_templateComboBox = WidgetsFactory::createComboBox(p_parent);

    // None.
    m_templateComboBox->addItem(tr("None"), "");

    int idx = 1;
    auto templates = TemplateMgr::getInst().getTemplates();
    for (const auto &temp : templates) {
        m_templateComboBox->addItem(temp, temp);
        m_templateComboBox->setItemData(idx++, temp, Qt::ToolTipRole);
    }

    m_templateComboBox->setCurrentIndex(0);

    connect(m_templateComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NewNoteDialog::updateCurrentTemplate);
}

QString NewNoteDialog::getTemplateContent() const
{
    int cursorOffset = 0;
    return SnippetMgr::getInst().applySnippetBySymbol(m_templateContent,
                                                      QString(),
                                                      cursorOffset,
                                                      SnippetMgr::generateOverrides(m_infoWidget->getName()));
}

void NewNoteDialog::updateCurrentTemplate()
{
    m_templateContent.clear();
    m_templateTextEdit->clear();

    auto temp = m_templateComboBox->currentData().toString();
    if (temp.isEmpty()) {
        m_templateTextEdit->hide();
        return;
    }

    const auto filePath = TemplateMgr::getInst().getTemplateFilePath(temp);
    try {
        m_templateContent = FileUtils::readTextFile(filePath);
        m_templateTextEdit->setPlainText(m_templateContent);
        m_templateTextEdit->show();
    } catch (Exception &p_e) {
        m_templateTextEdit->hide();

        QString msg = tr("Failed to load template (%1) (%2).")
            .arg(filePath, p_e.what());
        qCritical() << msg;
        setInformationText(msg, ScrollDialog::InformationLevel::Error);
    }
}
