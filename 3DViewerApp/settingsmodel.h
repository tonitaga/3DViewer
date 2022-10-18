#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

#include <QWidget>
#include <QDebug>


struct Model3DSettings {
    Model3DSettings() {

        vertex_color[0] = 1.0;
        vertex_color[1] = 0.0;
        vertex_color[2] = 0.0;

        rib_color[0] = 0.0;
        rib_color[1] = 1.0;
        rib_color[2] = 0.0;

        bg_color[0] = 0.0;
        bg_color[1] = 0.0;
        bg_color[2] = 0.0;

        vertex_size = 2.0f;
        rib_size = 0.5f;
        rib_type = 0;
        vertex_showness = 0;
        all_accept = false;
    }
    float vertex_color[3];
    float rib_color[3];
    float bg_color[3];
    float vertex_size;
    float rib_size;
    int rib_type;
    int vertex_showness;
    bool all_accept;
};


namespace Ui {
class SettingsModel;
}

class SettingsModel : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsModel(QWidget *parent = nullptr);
    ~SettingsModel();

signals:
    void window_before();
    void signalSettings(Model3DSettings *settings);

private:
    Ui::SettingsModel *ui;
    Model3DSettings *settings;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_pushButton_acceptAll_clicked();
    void on_pushButton_ribColorChoose_clicked();
    void on_pushButton_vertexColorChoose_clicked();
    void on_pushButton_bgColorChoose_clicked();
};

#endif // SETTINGSMODEL_H
