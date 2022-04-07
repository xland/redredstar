import { BaseModel } from './BaseModel'

export class SettingModel extends BaseModel {
  categoryWidth: number
  titleListWidth: number
  curCategoryId: string
  curArticleId: string
  windowIsMaximized: boolean
  windowWidth: number
  windowHeight: number
  autoSaveTime: number
}
