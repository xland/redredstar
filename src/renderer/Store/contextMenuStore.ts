import { writable } from 'svelte/store'
import { ContextMenuConfigModel } from '../../model/ContextMenuConfigModel'
import type { ContextMenuModel } from '../../model/ContextMenuModel'

export let contextMenuStore = writable<ContextMenuModel[]>([])
export let contextMenuConfigStore = writable<ContextMenuConfigModel>(null, (set) => {
  let config = new ContextMenuConfigModel()
  set(config)
  return () => {}
})
