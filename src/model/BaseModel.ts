import crypto from 'crypto'
export class BaseModel {
  id: string
  createTime: number
  updateTime: number
  getData() {
    let result: any = {}
    for (let key in this) {
      if (key.startsWith('_')) continue
      result[key] = this[key]
    }
    return result
  }
  constructor() {
    this.id = crypto.randomUUID()
  }
}
