#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(SmartPracticeLayer, PlayLayer) {
    struct Fields {
        float timer = 0.f;
        CCPoint lastPos = CCPointZero;
        CCLabelBMFont* warn = nullptr;
    };

    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (GameManager::sharedState()->getGameVariable("0071")) {
            geode::queueInMainThread([this]() {
                auto winSize = CCDirector::sharedDirector()->getWinSize();
                m_fields->warn = CCLabelBMFont::create("Smart Practice: Turn OFF Vanilla Auto-CP!", "bigFont.fnt");
                m_fields->warn->setID("warning-label"_spr);
                m_fields->warn->setColor({ 255, 50, 50 });
                m_fields->warn->setScale(0.5f);
                m_fields->warn->setPosition({ winSize.width / 2.f, winSize.height - 30.f });
                m_fields->warn->setZOrder(999);
                this->addChild(m_fields->warn);
                });
        }
        return true;
    }

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        auto pl = m_player1;
        if (!m_isPracticeMode || !pl || pl->m_isDead || m_hasCompletedLevel) return;

        m_fields->timer += dt;
        if (m_fields->timer < 1.15f) return;

        bool isGround = !pl->m_isShip && !pl->m_isBird && !pl->m_isDart && !pl->m_isSwing;

        bool isSafe = !pl->m_isDashing &&
            (isGround ? pl->m_isOnGround : (pl->m_isDart || std::abs(pl->m_yVelocity) < 4.5f));

        auto pos = pl->getPosition();
        if (std::abs(pos.x - m_fields->lastPos.x) < 280.f) isSafe = false;

        if (isSafe) {
            this->markCheckpoint();
            m_fields->timer = 0.f;
            m_fields->lastPos = pos;
        }
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        m_fields->timer = 0.f;
        if (m_player1) m_fields->lastPos = m_player1->getPosition();
    }
};