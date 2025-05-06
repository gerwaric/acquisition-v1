#pragma once

#include <libpoe/character.h>
#include <libpoe/stashtab.h>

#include <QObject>

#include <QSqlDatabase>
#include <QString>
#include <QStringList>
#include <QVariant>

class LeagueStore : public QObject {
    Q_OBJECT
public:
	explicit LeagueStore(const QString& directory, QObject *parent);

	void storeCharacter(const poe::Character& character);
	void storeStash(const poe::StashTab& stash_tab);

    QStringList getCharacterList();
    QStringList getStashList();

	poe::Character getCharacter(const QString& character_id);
	poe::StashTab getStash(const QString& stash_id);

	std::vector<poe::Character> getCharacters();
	std::vector<poe::StashTab> getStashes();

public slots:

    void setUsername(const QString& username);
    void setLeague(const QString& league);

private:

	QByteArray generateHash(const QString& data);

    QString m_data_directory;
    QString m_username;
    QString m_league;
    QSqlDatabase m_db;

};
