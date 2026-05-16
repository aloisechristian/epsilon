#ifndef GRAPH3D_APP_H
#define GRAPH3D_APP_H

#include <escher/app.h>
#include <escher/view_controller.h>
#include <escher/view.h>
#include <apps/i18n.h>
#include <apps/shared/shared_app.h>

namespace Graph3d {

class App : public Shared::SharedApp {
public:
  class Descriptor : public Escher::App::Descriptor {
  public:
    I18n::Message name() const override;
    I18n::Message upperName() const override;
    const Escher::Image * icon() const override;
  };

  class Snapshot : public Shared::SharedApp::Snapshot {
  public:
    Snapshot();
    App * unpack(Escher::Container * container) override;
    const Descriptor * descriptor() const override;
    void reset() override;
    void tidy() override;

    char m_surfaceExpression[256];
  };

  App(Snapshot * snapshot);
  virtual ~App() = default;

  Snapshot * snapshot() const {
    return static_cast<Snapshot*>(const_cast<Escher::App::Snapshot*>(Escher::App::snapshot()));
  }

private:
  class MainView : public Escher::View {
  public:
    MainView(App * app) : m_app(app), m_theta(0.5f), m_phi(0.5f) {}
    void drawRect(KDContext * ctx, KDRect rect) const override;
    void setRotation(float dTheta, float dPhi) {
      m_theta += dTheta;
      m_phi += dPhi;
      markRectAsDirty(bounds());
    }
  private:
    App * m_app;
    float m_theta;
    float m_phi;
  };

  class MainViewController : public Escher::ViewController {
  public:
    MainViewController(Escher::Responder * parentResponder, App * app) : Escher::ViewController(parentResponder), m_view(app) {}
    Escher::View * view() override { return &m_view; }
    const char * title() const override { return "Graph3D"; }
    bool handleEvent(Ion::Events::Event event) override;
  private:
    MainView m_view;
  };

  MainViewController m_mainViewController;
};

}

#endif
