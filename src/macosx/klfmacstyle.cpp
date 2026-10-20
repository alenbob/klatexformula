// macOS presentation for the existing equation editor and export actions.
// This file is part of KLatexFormula, licensed under GPL version 2 or later.

#include <QtWidgets>
#include <ui_klfmainwin.h>

#include "klfmainwin.h"

namespace {
QLabel *heading(const QString& text, const char *name, QWidget *parent)
{
  QLabel *label = new QLabel(text, parent);
  label->setObjectName(QLatin1String(name));
  return label;
}

void textButton(QPushButton *button, const QString& text)
{
  button->setText(text);
  button->setIcon(QIcon());
  button->setMinimumHeight(24);
  button->setMaximumHeight(QWIDGETSIZE_MAX);
  button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}
}

void KLFMainWin::setupMacAppearance()
{
  const QFont systemFont = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
  setFont(systemFont);
  u->lyt_KLFMainWin->setContentsMargins(10, 8, 10, 10);
  u->lyt_KLFMainWin->setSpacing(10);
  u->lyt_frmMain->setContentsMargins(0, 0, 0, 0);
  u->lyt_frmMain->setSpacing(8);
  u->btnQuit->hide(); // macOS provides Quit in the application menu and Cmd-Q.

  QWidget *header = new QWidget(u->frmMain);
  header->setObjectName("macHeader");
  QHBoxLayout *headerLayout = new QHBoxLayout(header);
  headerLayout->setContentsMargins(0, 0, 0, 0);
  headerLayout->setSpacing(6);
  headerLayout->addStretch();
  const QList<QPushButton *> navigation = {u->btnSymbols, u->btnLibrary, u->btnExpand};
  const QStringList names = {tr("Symbols"), tr("Library"), tr("Options")};
  for (int index = 0; index < navigation.size(); ++index) {
    u->lytMiddle->removeWidget(navigation[index]);
    textButton(navigation[index], names[index]);
    headerLayout->addWidget(navigation[index]);
  }
  u->lyt_frmMain->insertWidget(0, header);

  // Wrap the existing editor, preserving its shortcuts, highlighting and drops.
  QWidget *editorPanel = new QWidget(u->splitMain);
  editorPanel->setObjectName("macEditorPanel");
  QVBoxLayout *editorLayout = new QVBoxLayout(editorPanel);
  editorLayout->setContentsMargins(10, 8, 10, 8);
  editorLayout->setSpacing(6);
  QHBoxLayout *editorHeading = new QHBoxLayout;
  editorHeading->addWidget(heading(tr("LaTeX"), "macSectionTitle", editorPanel));
  editorHeading->addStretch();
  editorHeading->addWidget(heading(tr("Shift + Return to render"), "macHint", editorPanel));
  editorLayout->addLayout(editorHeading);
  u->txtLatex->setParent(editorPanel);
  u->txtLatex->setFrameShape(QFrame::NoFrame);
  u->txtLatex->setMinimumHeight(70);
  u->txtLatex->setPlaceholderText(tr("Type an equation, for example E = mc^2"));
  u->txtLatex->setFont(systemFont);
  u->txtPreamble->setFont(systemFont);
  editorLayout->addWidget(u->txtLatex);
  u->splitMain->insertWidget(0, editorPanel);

  textButton(u->btnClearAll, tr("Clear"));
  textButton(u->btnClearLatex, tr("Clear"));
  textButton(u->btnEvaluate, tr("Render"));
  u->lytMiddle->removeWidget(u->btnEvaluate);
  u->lytMiddle->addStretch();
  u->lytMiddle->addWidget(u->btnEvaluate);
  u->lytMiddle->setSpacing(6);
  u->lytMiddle->setContentsMargins(0, 0, 0, 8);
  u->vlyt_OutputFrame->setSpacing(0);

  // Replace the vertical export stack with a single, labeled action bar.
  while (QLayoutItem *item = u->lyt_frmOutput->takeAt(0)) {
    delete item; // Layout items do not own the widgets.
  }
  u->lyt_frmOutput->setContentsMargins(10, 8, 10, 8);
  u->lyt_frmOutput->setSpacing(6);
  u->lyt_frmOutput->setColumnStretch(0, 1);
  u->lyt_frmOutput->setColumnStretch(1, 0);
  u->lyt_frmOutput->addWidget(heading(tr("Preview"), "macSectionTitle", u->frmOutput), 0, 0);
  u->lblOutput->setMinimumSize(300, 80);
  u->lblOutput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  u->lblOutput->setMargin(6);
  u->lblOutput->setAccessibleName(tr("Rendered equation preview"));
  u->lyt_frmOutput->addWidget(u->lblOutput, 1, 0);
  u->lyt_frmOutput->setRowStretch(1, 1);

  QHBoxLayout *exportLayout = new QHBoxLayout;
  exportLayout->setSpacing(6);
  textButton(u->btnDrag, tr("Drag"));
  textButton(u->btnCopy, tr("Copy"));
  textButton(u->btnSave, tr("Save…"));
  exportLayout->addWidget(u->btnCopy);
  exportLayout->addWidget(u->btnSave);
  exportLayout->addWidget(u->btnDrag);
  exportLayout->addStretch();
  u->lyt_wForBigPreviewButton->removeWidget(u->btnShowBigPreview);
  u->lyt_wForBigPreviewButton->removeWidget(u->btnSetExportProfile);
  textButton(u->btnShowBigPreview, tr("Zoom"));
  exportLayout->addWidget(u->btnShowBigPreview);
  u->btnSetExportProfile->setIcon(QIcon());
  u->btnSetExportProfile->setText(tr("Export format"));
  u->btnSetExportProfile->setToolButtonStyle(Qt::ToolButtonTextOnly);
  u->btnSetExportProfile->setMinimumHeight(24);
  exportLayout->addWidget(u->btnSetExportProfile);
  u->wForBigPreviewButton->hide();
  u->lyt_frmOutput->addLayout(exportLayout, 2, 0);

  // NSDrawer predates modern macOS; keep options in the resizable main window.
  if (u->frmDetails->parentWidget() != u->wCentralWidget) {
    u->frmDetails->setParent(u->wCentralWidget);
    u->lyt_KLFMainWin->addWidget(u->frmDetails);
  }
  u->frmDetails->setSideWidgetManager(QLatin1String("ShowHide"));
  u->frmDetails->setMinimumWidth(280);
  u->frmDetails->setMaximumWidth(320);
  u->lyt_KLFMainWin->setStretch(0, 1);
  u->lyt_KLFMainWin->setStretch(1, 0);
  for (QComboBox *combo : u->frmDetails->findChildren<QComboBox *>()) {
    combo->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    combo->setMinimumContentsLength(10);
  }
  u->splitMain->setChildrenCollapsible(false);
  u->splitMain->setHandleWidth(6);
  u->splitMain->setStretchFactor(0, 1);
  u->splitMain->setStretchFactor(1, 1);
  u->splitMain->setSizes(QList<int>() << 160 << 210);
  setMinimumSize(560, 390);
  resize(620, 460);
  refreshMacAppearance();

  extern void klf_mac_modern_window(QWidget *);
  klf_mac_modern_window(this);
}

void KLFMainWin::refreshMacAppearance()
{
  const bool dark = QApplication::palette().color(QPalette::Window).lightness() < 128;
  const QString background = dark ? "#202329" : "#edf0f5";
  const QString panel = dark ? "#2c3038" : "#ffffff";
  const QString text = dark ? "#f4f6fb" : "#192332";
  const QString muted = dark ? "#b5bfce" : "#536176";
  const QString border = dark ? "#788393" : "#919dad";
  const QString softBorder = dark ? "#505967" : "#ccd3dd";
  const QString button = dark ? "#3b424e" : "#ffffff";
  const QString hover = dark ? "#4a5565" : "#e5edf9";
  const QString pressed = dark ? "#58687d" : "#d2e0f5";
  const QString disabled = dark ? "#a6afbd" : "#626e7f";
  const QString disabledBackground = dark ? "#303640" : "#e5e9ef";
  // Legacy syntax colors were chosen for a white editor. Keep a copy of the
  // user's formats and adapt their foregrounds when the system uses dark mode.
  for (KLFLatexEdit *editor : {u->txtLatex, u->txtPreamble}) {
    KLFLatexSyntaxHighlighter *highlighter = editor->syntaxHighlighter();
    const QStringList formats = {"fmtKeyword", "fmtComment", "fmtParenMatch",
                                 "fmtParenMismatch", "fmtLonelyParen"};
    for (const QString& format : formats) {
      const QByteArray name = format.toLatin1();
      const QByteArray saved = "macOriginal_" + name;
      if (!highlighter->property(saved).isValid())
        highlighter->setProperty(saved, highlighter->property(name));
      QTextCharFormat value = highlighter->property(saved).value<QTextFormat>().toCharFormat();
      if (dark) {
        if (format == "fmtKeyword") value.setForeground(QColor("#9fc5ff"));
        if (format == "fmtComment") value.setForeground(QColor("#ffb8b0"));
        if (format == "fmtLonelyParen") value.setForeground(QColor("#f6b0ff"));
        if (format == "fmtParenMatch") {
          value.setForeground(QColor("#f4f6fb"));
          value.setBackground(QColor("#365745"));
        }
        if (format == "fmtParenMismatch") {
          value.setForeground(QColor("#ffffff"));
          value.setBackground(QColor("#853e62"));
        }
      }
      highlighter->setProperty(name, QVariant::fromValue(QTextFormat(value)));
    }
    highlighter->rehighlight();
  }
  QString css = QString::fromLatin1(R"CSS(
    QMainWindow#KLFMainWin { background: @background; color: @text; }
    KLFLatexSymbols, QWidget#frmSymbolList { background: @background; color: @text; }
    QLabel { color: @text; }
    QLabel#macHint { color: @muted; }
    QWidget#macEditorPanel, QFrame#frmOutput {
      background: @panel; border: 1px solid @softBorder; border-radius: 5px;
    }
    QTextEdit#txtLatex {
      background: @panel; color: @text; border: none; padding: 2px;
      selection-background-color: #075dc8; selection-color: white;
    }
    QTextEdit#txtLatex:focus { border-bottom: 2px solid #377fdd; }
    QLabel#lblOutput {
      background: #ffffff; color: #25334a; border: 1px solid @softBorder;
      border-radius: 3px;
    }
    QLabel#lblOutput[realTimeLatexError="true"] {
      background: #fff1f0; color: #9b1c13; border: 1px solid #b42318;
    }
    QPushButton, QToolButton {
      background: @button; color: @text; border: 1px solid @border;
      border-radius: 4px; padding: 0px 8px; min-height: 22px;
      outline: none;
    }
    QPushButton:hover, QToolButton:hover { background: @hover; }
    QPushButton:pressed, QToolButton:pressed { background: @pressed; }
    QPushButton:checked { background: @pressed; border: 1px solid #377fdd; }
    QPushButton:focus, QToolButton:focus { border: 2px solid #377fdd; padding: 0px 7px; }
    QPushButton#btnEvaluate {
      background: #075dc8; color: white; border: 1px solid #0751ae;
      border-radius: 4px; padding: 0px 12px; min-height: 22px;
    }
    QPushButton#btnEvaluate:hover { background: #064fae; }
    QPushButton#btnEvaluate:pressed { background: #04418e; }
    QPushButton#btnEvaluate:focus { border: 2px solid @text; padding: 0px 11px; }
    QPushButton:disabled, QToolButton:disabled, QPushButton#btnEvaluate:disabled {
      color: @disabled; background: @disabledBackground; border: 1px solid @softBorder;
    }
    KLFPixmapButton {
      background: #ffffff; color: #192332; border: 1px solid #919dad;
      border-radius: 4px; padding: 2px; min-height: 24px;
    }
    KLFPixmapButton:hover { background: #e5edfa; border: 1px solid #075dc8; }
    KLFPixmapButton:pressed { background: #c6dcff; }
    QTabWidget::pane {
      background: @panel; border: 1px solid @softBorder; border-radius: 4px;
      top: -1px;
    }
    QTabBar::tab {
      background: @button; color: @text; border: 1px solid @border;
      border-radius: 4px; padding: 4px 6px; margin: 0px 2px 4px 0px;
    }
    QTabBar::tab:selected { background: @pressed; border: 1px solid #377fdd; }
    QTabBar::tab:hover { background: @hover; }
    QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QTextEdit#txtPreamble {
      background: @panel; color: @text; border: 1px solid @border;
      border-radius: 3px; padding: 2px 5px; min-height: 20px;
      selection-background-color: #075dc8; selection-color: white;
    }
    QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QTextEdit#txtPreamble:focus {
      border: 1px solid #377fdd;
    }
    QComboBox QLineEdit { border: none; padding: 0px; min-height: 0px; background: transparent; }
    QComboBox::drop-down { border: none; width: 24px; }
    QComboBox::down-arrow { image: url(@chevron); width: 12px; height: 12px; }
    QComboBox QAbstractItemView { background: @panel; color: @text; selection-background-color: #075dc8; selection-color: white; }
    QToolButton::menu-indicator, QPushButton::menu-indicator {
      image: url(@chevron); width: 10px; height: 10px;
      subcontrol-position: right center; subcontrol-origin: padding;
    }
    QGroupBox {
      color: @text; border: 1px solid @softBorder; border-radius: 4px;
      margin-top: 8px; padding-top: 8px;
    }
    QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0px 4px; }
    QSplitter::handle { background: transparent; }
    QToolTip { background: @panel; color: @text; border: 1px solid @border; padding: 6px; }
  )CSS");
  const QList<QPair<QString, QString> > tokens = {
    {"@background", background}, {"@panel", panel}, {"@text", text},
    {"@muted", muted}, {"@border", border}, {"@softBorder", softBorder},
    {"@button", button}, {"@hover", hover}, {"@pressed", pressed},
    {"@chevron", dark ? ":/pics/macos-chevron-dark.svg" : ":/pics/macos-chevron-light.svg"},
    {"@disabledBackground", disabledBackground}, {"@disabled", disabled}
  };
  for (const auto& token : tokens) css.replace(token.first, token.second);
  // Auxiliary windows are independent top-level widgets on macOS. Apply the
  // same controls there, including white tiles for black symbol thumbnails.
  if (qApp->styleSheet() != css)
    qApp->setStyleSheet(css);
}
