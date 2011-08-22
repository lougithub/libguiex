<?xml version="1.0" ?>
<guiex>
	<property name="as_panel_lower" type="CGUIAsDefine" value="CGUIAsContainer">
		<property name="total_time" type="real" value="2" />

		<property name="item_info" type="group">
			<property name="begin_time" type="real" value="0"/>
			<property name="item" type="CGUIAsDefine" value="CGUIAsWidgetChildrenVisible">
				<property name="total_time" type="real" value="0" />
				<property name="visibility" type="bool" value="false" />
			</property>
		</property>

		<property name="item_info" type="group">
			<property name="begin_time" type="real" value="0"/>
			<property name="item" type="CGUIAsDefine" value="CGUIAsWidgetAlpha">
				<property name="total_time" type="real" value="1" />
				<property name="interpolation" type="EInterpolationType" value="eInterpolationType_EaseInOut" />
				<property name="begin_value" type="real" value="0" />
				<property name="end_value" type="real" value="1" />
			</property>
		</property>

		<property name="item_info" type="group">
			<property name="begin_time" type="real" value="1"/>
			<property name="item" type="CGUIAsDefine" value="CGUIAsWidgetChildrenVisible">
				<property name="total_time" type="real" value="0" />
				<property name="visibility" type="bool" value="true" />
			</property>
		</property>

		<property name="item_info" type="group">
			<property name="begin_time" type="real" value="1"/>
			<property name="item" type="CGUIAsDefine" value="CGUIAsWidgetChildrenAlpha">
				<property name="total_time" type="real" value="1" />
				<property name="interpolation" type="EInterpolationType" value="eInterpolationType_EaseInOut" />
				<property name="begin_value" type="real" value="0" />
				<property name="end_value" type="real" value="1" />
			</property>
		</property>

	</property>

</guiex>
