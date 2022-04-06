import events from 'events'
class Eventer {
  private instance: events.EventEmitter
  emit(name: string, ...params) {
    this.instance.emit(name, ...params)
  }
  on(name: string, callback: (...args: any[]) => void) {
    this.instance.on(name, callback)
  }
  off(name: string, callback: (...args: any[]) => void) {
    this.instance.off(name, callback)
  }
  once(name, callback: (...args: any[]) => void) {
    this.instance.once(name, callback)
  }
  constructor() {
    this.instance = new events.EventEmitter()
  }
}
export let eventer = new Eventer()
