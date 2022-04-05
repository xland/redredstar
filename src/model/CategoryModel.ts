export class CategoryModel {
  title: string
  id: string
  parentId: string
  order: number
  createTime: number
  updateTime: number
  subCategory: CategoryModel[]
  level: number
  isExpanded: boolean
  isSelected: boolean
}
