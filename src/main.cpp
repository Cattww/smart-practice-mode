#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(SmartPracticeLayer, PlayLayer) {

    struct Fields {
        float m_smartTimer = 0.f;
        cocos2d::CCPoint m_lastCPPos = cocos2d::CCPointZero;
        cocos2d::CCLabelBMFont* m_warningLabel = nullptr;
    };

    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        // check vanilla auto-cp
        if (GameManager::sharedState()->getGameVariable("0071")) {
            auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

            m_fields->m_warningLabel = cocos2d::CCLabelBMFont::create(
                "Smart Practice: Turn OFF Vanilla Auto-CP!",
                "bigFont.fnt"
            );

            m_fields->m_warningLabel->setID("warning-label"_spr);

            m_fields->m_warningLabel->setColor({ 255, 50, 50 });
            m_fields->m_warningLabel->setScale(0.5f);
            m_fields->m_warningLabel->setPosition({ winSize.width / 2.f, winSize.height - 30.f });
            m_fields->m_warningLabel->setZOrder(999);

            this->addChild(m_fields->m_warningLabel);

            // TODO: add CCFadeOut so it doesn't overlap progress bar on ultrawide monitors
        }

        return true;
    }

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        // basic state checks
        if (!m_isPracticeMode || !m_player1 || m_player1->m_isDead || m_hasCompletedLevel) return;

        // tick timer
        m_fields->m_smartTimer += dt;

        // 1.15 feels better than 1.5 in fast parts
        float timerThreshold = static_cast<float>(Mod::get()->getSettingValue<double>("smart-timer"));

        if (m_fields->m_smartTimer >= timerThreshold) {
            bool isSafe = true;

            if (m_player1->m_isDashing) isSafe = false;

            bool isGroundMode = !m_player1->m_isShip && !m_player1->m_isBird && !m_player1->m_isDart && !m_player1->m_isSwing;

            if (isGroundMode) {
                if (!m_player1->m_isOnGround) isSafe = false;
            }
            else {
                // skip wave
                if (!m_player1->m_isDart) {
                    if (m_player1->m_yVelocity > 4.5f || m_player1->m_yVelocity < -4.5f) {
                        isSafe = false;
                    }
                }
            }

            // distance check (280 is roughly 9 blocks)
            float distThreshold = static_cast<float>(Mod::get()->getSettingValue<double>("safe-distance"));
            auto pos = m_player1->getPosition();

            if (std::abs(pos.x - m_fields->m_lastCPPos.x) < distThreshold) {
                isSafe = false;
            }

            if (isSafe) {
                this->markCheckpoint();
                m_fields->m_smartTimer = 0.f;
                m_fields->m_lastCPPos = pos;
            }
        }
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        m_fields->m_smartTimer = 0.f;

        // update last pos
        if (m_player1) {
            m_fields->m_lastCPPos = m_player1->getPosition();
        }
    }
};