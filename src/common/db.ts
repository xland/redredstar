import { Knex, knex } from 'knex'
import path from 'path'
let getDbPath = () => {
  let dbPath
  if (location.href.startsWith('http://localhost')) {
    dbPath = path.join(process.execPath, '../../../../src/common/db.db')
  } else {
    dbPath = path.join(process.execPath, '../../../../src/common/db.db')
  }
  return dbPath
}
let initDb = () => {
  let filename = getDbPath()
  let db = knex({
    client: 'better-sqlite3',
    connection: {
      filename,
    },
    useNullAsDefault: true,
  })
  return db
}
export let db = initDb()
