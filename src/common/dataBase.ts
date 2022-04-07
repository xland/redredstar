import { Knex, knex } from 'knex'
import path from 'path'
class DataBase {
  private db: Knex
  private getDbPath() {
    let dbPath
    if (location.href.startsWith('http://localhost')) {
      dbPath = path.join(process.execPath, '../../../../src/common/db.db')
    } else {
      dbPath = path.join(process.execPath, '../../../../src/common/db.db')
    }
    return dbPath
  }
  get() {
    if (this.db) return this.db
    let filename = this.getDbPath()
    this.db = knex({
      client: 'better-sqlite3',
      connection: {
        filename,
      },
      useNullAsDefault: true,
    })
    return this.db
  }
}

export let dataBase = new DataBase()
